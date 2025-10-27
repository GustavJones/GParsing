#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/EXIF/EXIFImageBase.hpp"
#include "GParsing/EXIF/EXIFDefinitions.hpp"
#include "GParsing/EXIF/EXIFDataType.hpp"
#include "GParsing/EXIF/EXIFTag.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

namespace GParsing {
	class GPARSING_API EXIFImageJPEG : public EXIFImageBase
	{
	private:
		bool _IsMarkerValid(const uint16_t _marker) {
			// Doesn't have marker prefix
			if ((_marker & 0xFF00) != 0xFF00)
			{
				return false;
			}

			// Actual 0xFF value in entropy-coded segment
			if ((_marker & 0x00FF) == 0x00)
			{
				return false;
			}

			// Temp value
			if ((_marker & 0x00FF) == 0x01)
			{
				return false;
			}

			// Reserved
			if ((_marker & 0x00FF) >= 0x02 && (_marker & 0x00FF) <= 0xBF)
			{
				return false;
			}

			// Sequences of 0xFF equals a single 0xFF
			if ((_marker & 0x00FF) == 0xFF)
			{
				return false;
			}

			return true;
		}

		uint16_t _ParseMarker(const std::vector<unsigned char>& _imageBuffer, const size_t _markerIndex) {
			if ((_markerIndex + 1) >= _imageBuffer.size() - 1)
			{
				return 0;
			}

			return BytesFromBuffer<unsigned char, uint16_t>(_imageBuffer, _markerIndex, Endianness::BIG);
		}

		uint16_t _ParseSegmentSize(const std::vector<unsigned char>& _imageBuffer, const size_t _markerIndex) {
			if (_markerIndex + 3 >= _imageBuffer.size() - 1)
			{
				return false;
			}

			return BytesFromBuffer<unsigned char, uint16_t>(_imageBuffer, _markerIndex + 2, Endianness::BIG) - 2;
		}

		bool _ParseSegmentBuffer(const std::vector<unsigned char>& _imageBuffer, const size_t _markerIndex, std::vector<unsigned char>& _markerBuffer) {
			uint16_t size = _ParseSegmentSize(_imageBuffer, _markerIndex);
			if (size <= 0)
			{
				return false;
			}

			_markerBuffer = std::vector<unsigned char>(_imageBuffer.begin() + _markerIndex + 4, _imageBuffer.begin() + _markerIndex + size + 4);
			return true;
		}

		bool _ParseNextSegmentIndex(const std::vector<unsigned char>& _imageBuffer, const size_t _markerIndex, size_t & _nextSegmentIndex) {
			uint16_t size = _ParseSegmentSize(_imageBuffer, _markerIndex);
			if (size <= 0)
			{
				return false;
			}

			_nextSegmentIndex = _markerIndex + size + 4;

			if (_nextSegmentIndex >= _imageBuffer.size())
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		bool _FindEXIFBuffer(const std::vector<unsigned char>& _imageBuffer, const size_t _startMarkerIndex, std::vector<unsigned char>& _exifBuffer) {
			size_t markerIndex = _startMarkerIndex;
			bool found = false;
			while (!found)
			{
				uint16_t marker = _ParseMarker(_imageBuffer, markerIndex);
				if (!_IsMarkerValid(marker))
				{
					return false;
				}

				if (marker != JPEGApplication1Marker)
				{
					if (!_ParseNextSegmentIndex(_imageBuffer, markerIndex, markerIndex))
					{
						return false;
					}
				}
				else
				{
					if (!_ParseSegmentBuffer(_imageBuffer, markerIndex, _exifBuffer))
					{
						return false;
					}

					found = true;
				}
			}

			return found;
		}

		bool _ParseIFD(const std::vector<unsigned char> &_exifBuffer, const uint16_t _ifdStartOffset, const size_t _ifdEntryCount, Endianness _endianness, uint32_t &_ifdEndOffset) {
			const size_t ENTRY_SIZE = 12;

			uint16_t tagID;
			EXIFDataType dataType;
			size_t totalSize;
			size_t dataTypeSize;
			uint32_t componentsCount;
			std::vector<unsigned char> data;

			std::vector<uint8_t> component;
			EXIFTag<uint8_t> tag;

			_ifdEndOffset = BytesFromBuffer<unsigned char, uint32_t>(_exifBuffer, _ifdStartOffset + (_ifdEntryCount * ENTRY_SIZE) + 2, _endianness);

			if (_endianness == Endianness::LITTLE)
			{
				for (size_t i = 0; i < _ifdEntryCount; i++)
				{
					std::vector<unsigned char> entry(_exifBuffer.begin() + _ifdStartOffset + (i * ENTRY_SIZE) + 2, _exifBuffer.begin() + _ifdStartOffset + ((i + 1) * ENTRY_SIZE) + 2);

					tagID = BytesFromBuffer<unsigned char, uint16_t>(entry, 0, Endianness::LITTLE);
					dataType = static_cast<EXIFDataType>(BytesFromBuffer<unsigned char, uint16_t>(entry, 2, Endianness::LITTLE));
					dataTypeSize = EXIFDataTypeSizes.at(dataType);
					componentsCount = BytesFromBuffer<unsigned char, uint32_t>(entry, 4, Endianness::LITTLE);
					totalSize = dataTypeSize * componentsCount;
					data = std::vector<unsigned char>(entry.begin() + 8, entry.end());

					tag.SetID(static_cast<EXIFTagNumber>(tagID));

					tag.SetType(dataType);
					component.resize(dataTypeSize);

					if (totalSize <= 4)
					{
						for (size_t i = 0; i < componentsCount; i++)
						{
							for (size_t j = 0; j < component.size(); j++)
							{
								component[component.size() - j - 1] = data[i * dataTypeSize + j];
							}

							tag.AppendComponent(component);
						}

						AppendTag(tag);
						tag.ClearComponents();
					}
					else
					{
						uint32_t dataOffset = BytesFromBuffer<unsigned char, uint32_t>(data, 0, Endianness::LITTLE);

						for (size_t i = 0; i < componentsCount; i++)
						{
							for (size_t j = 0; j < component.size(); j++)
							{
								component[component.size() - j - 1] = _exifBuffer[dataOffset + (i * dataTypeSize + j)];
							}

							tag.AppendComponent(component);
						}

						AppendTag(tag);
						tag.ClearComponents();
					}
				}
			}
			else if (_endianness == Endianness::BIG)
			{
				for (size_t i = 0; i < _ifdEntryCount; i++)
				{
					std::vector<unsigned char> entry(_exifBuffer.begin() + _ifdStartOffset + (i * ENTRY_SIZE) + 2, _exifBuffer.begin() + _ifdStartOffset + ((i + 1) * ENTRY_SIZE) + 2);

					tagID = BytesFromBuffer<unsigned char, uint16_t>(entry, 0, Endianness::BIG);
					dataType = static_cast<EXIFDataType>(BytesFromBuffer<unsigned char, uint16_t>(entry, 2, Endianness::BIG));
					dataTypeSize = EXIFDataTypeSizes.at(dataType);
					componentsCount = BytesFromBuffer<unsigned char, uint32_t>(entry, 4, Endianness::BIG);
					totalSize = dataTypeSize * componentsCount;
					data = std::vector<unsigned char>(entry.begin() + 8, entry.end());

					tag.SetID(static_cast<EXIFTagNumber>(tagID));

					tag.SetType(dataType);
					component.resize(dataTypeSize);

					if (totalSize <= 4)
					{
						for (size_t i = 0; i < componentsCount; i++)
						{
							for (size_t j = 0; j < component.size(); j++)
							{
								component[j] = data[i * dataTypeSize + j];
							}

							tag.AppendComponent(component);
						}

						AppendTag(tag);
						tag.ClearComponents();
					}
					else
					{
						uint32_t dataOffset = BytesFromBuffer<unsigned char, uint32_t>(data, 0, Endianness::BIG);

						component.resize(dataTypeSize);

						for (size_t i = 0; i < componentsCount; i++)
						{
							for (size_t j = 0; j < component.size(); j++)
							{
								component[j] = _exifBuffer[dataOffset + (i * dataTypeSize + j)];
							}

							tag.AppendComponent(component);
						}

						AppendTag(tag);
						tag.ClearComponents();
					}
				}
			}
			else
			{
				return false;
			}

			return true;
		}

	public:
		bool Parse(const std::vector<unsigned char>& _imageBuffer) override {
			std::vector<unsigned char> exifBuffer;

			if (_imageBuffer.size() == 0)
			{
				return false;
			}

			if (_ParseMarker(_imageBuffer, 0) != GParsing::JPEGStartOfImageMarker)
			{
				return false;
			}

			if (!_FindEXIFBuffer(_imageBuffer, 2, exifBuffer))
			{
				return false;
			}

			if (!CaseSensitiveMatch(exifBuffer.data(), reinterpret_cast<unsigned char *>("Exif\0\0"), 6))
			{
				return false;
			}

			exifBuffer.erase(exifBuffer.begin(), exifBuffer.begin() + 6);

			uint32_t IFDOffset;
			uint16_t IFDEntryCount;

			const bool littleEndian = CaseSensitiveMatch(exifBuffer.data(), reinterpret_cast<unsigned char*>("II"), 2);
			const bool bigEndian = CaseSensitiveMatch(exifBuffer.data(), reinterpret_cast<unsigned char*>("MM"), 2);

			if (littleEndian)
			{
				if (!CaseSensitiveMatch(exifBuffer.data() + 2, reinterpret_cast<unsigned char*>("\x2A\x00"), 2))
				{
					return false;
				}

				IFDOffset = BytesFromBuffer<unsigned char, uint16_t>(exifBuffer, 4, Endianness::LITTLE);

				while (IFDOffset > 0)
				{
					IFDEntryCount = BytesFromBuffer<unsigned char, uint16_t>(exifBuffer, IFDOffset, Endianness::LITTLE);

					if (!_ParseIFD(exifBuffer, IFDOffset, IFDEntryCount, Endianness::LITTLE, IFDOffset))
					{
						return false;
					}
				}

				for (size_t i = 0; i < tags.size(); i++)
				{
					if (tags[i].GetID() == EXIFTagNumber::EXIFOffset_MainImage)
					{
						if (tags[i].GetComponentsCount() == 1)
						{
							IFDOffset = BytesFromBuffer<unsigned char, uint32_t>(tags[i].GetComponentFromIndex(0), 0, Endianness::BIG);
							break;
						}
					}
				}

				while (IFDOffset > 0)
				{
					IFDEntryCount = BytesFromBuffer<unsigned char, uint16_t>(exifBuffer, IFDOffset, Endianness::LITTLE);

					if (!_ParseIFD(exifBuffer, IFDOffset, IFDEntryCount, Endianness::LITTLE, IFDOffset))
					{
						return false;
					}
				}
			}
			else if (bigEndian)
			{
				if (!CaseSensitiveMatch(exifBuffer.data() + 2, reinterpret_cast<unsigned char*>("\x00\x2A"), 2))
				{
					return false;
				}

				IFDOffset = BytesFromBuffer<unsigned char, uint16_t>(exifBuffer, 4, Endianness::BIG);

				while (IFDOffset > 0)
				{
					IFDEntryCount = BytesFromBuffer<unsigned char, uint16_t>(exifBuffer, IFDOffset, Endianness::BIG);

					if (!_ParseIFD(exifBuffer, IFDOffset, IFDEntryCount, Endianness::BIG, IFDOffset))
					{
						return false;
					}
				}

				for (size_t i = 0; i < tags.size(); i++)
				{
					if (tags[i].GetID() == EXIFTagNumber::EXIFOffset_MainImage)
					{
						if (tags[i].GetComponentsCount() == 1)
						{
							IFDOffset = BytesFromBuffer<unsigned char, uint32_t>(tags[i].GetComponentFromIndex(0), 0, Endianness::BIG);
							break;
						}
					}
				}

				while (IFDOffset > 0)
				{
					IFDEntryCount = BytesFromBuffer<unsigned char, uint16_t>(exifBuffer, IFDOffset, Endianness::BIG);

					if (!_ParseIFD(exifBuffer, IFDOffset, IFDEntryCount, Endianness::BIG, IFDOffset))
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}

			return true;
		}
	};
}
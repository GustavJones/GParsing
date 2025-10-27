#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/EXIF/EXIFTag.hpp"
#include <vector>

namespace GParsing {
	class GPARSING_API EXIFImageBase
	{
	public:
		virtual bool Parse(const std::vector<unsigned char>& _imageBuffer) = 0;

		std::vector<GParsing::EXIFTag<uint8_t>> tags;

		void AppendTag(const GParsing::EXIFTag<uint8_t>& _tag) {
			bool duplicate = false;

			for (size_t i = 0; i < tags.size(); i++)
			{
				if (tags[i].GetID() == _tag.GetID())
				{
					duplicate = true;
					break;
				}
			}

			if (!duplicate)
			{
				tags.push_back(_tag);
			}
		}
	};
}
#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/EXIF/EXIFTag.hpp"
#include <vector>

namespace GParsing {
	template<typename ByteT>
	class GPARSING_API EXIFImageBase
	{
	public:
		std::vector<GParsing::EXIFTag<ByteT>> tags;

		virtual bool Parse(const std::vector<unsigned char>& _imageBuffer) = 0;

		void AppendTag(const GParsing::EXIFTag<ByteT>& _tag) {
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

		std::vector<GParsing::EXIFTag<ByteT>> GetTags(const GParsing::EXIFTagNumber& _tagID) const {
			std::vector<GParsing::EXIFTag<ByteT>> output;

			for (const auto& tag : tags)
			{
				if (tag.GetID() == _tagID)
				{
					output.push_back(tag);
				}
			}

			return output;
		}
	};
}
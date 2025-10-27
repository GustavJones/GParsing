#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/EXIF/EXIFDataType.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace GParsing {
	enum class EXIFTagNumber : uint16_t
	{
		UNKNOWN = 0x0000,

		// No IFD Listed
		FreeOffsets_None = 0x0120,
		FreeByteCounts_None = 0x0121,
		GrayResponseCurve_None = 0x0123,
		T4Options_None = 0x0124,
		T6Options_None = 0x0125,
		ColorResponseUnit_None = 0x012C,
		ColorMap_None = 0x0140,
		TileOffsets_None = 0x0144,
		TileByteCounts_None = 0x0145,
		BadFaxLines_None = 0x0146,
		CleanFaxData_None = 0x0147,
		ConsecutiveBadFaxLines_None = 0x0148,
		SubIFD_None = 0x014A,
		InkNames_None = 0x014D,
		NumberofInks_None = 0x014E,
		DotRange_None = 0x0150,
		ExtraSamples_None = 0x0152,
		SMinSampleValue_None = 0x0154,
		SMaxSampleValue_None = 0x0155,
		TransferRange_None = 0x0156,
		ClipPath_None = 0x0157,
		XClipPathUnits_None = 0x0158,
		YClipPathUnits_None = 0x0159,
		Indexed_None = 0x015A,
		JPEGTables1_None = 0x015B,
		OPIProxy_None = 0x015F,
		GlobalParametersIFD_None = 0x0190,
		ProfileType_None = 0x0191,
		FaxProfile_None = 0x0192,
		CodingMethods_None = 0x0193,
		VersionYear_None = 0x0194,
		ModeNumber_None = 0x0195,
		Decode_None = 0x01B1,
		DefaultImageColor_None = 0x01B2,
		T82Options_None = 0x01B3,
		JPEGTables2_None = 0x01B5,
		JPEGProc_None = 0x0200,
		JPEGRestartInterval_None = 0x0203,
		JPEGLosslessPredictors_None = 0x0205,
		JPEGPointTransforms_None = 0x0206,
		JPEGQTables_None = 0x0207,
		JPEGDCTables_None = 0x0208,
		JPEGACTables_None = 0x0209,
		StripRowCounts_None = 0x022F,
		RenderingIntent_None = 0x0303,
		USPTOMiscellaneous_None = 0x03E7,
		XP_DIP_XML_None = 0x4747,
		StitchInfo_None = 0x4748,
		// TODO 0x5001 - 0x5113
		SonyRawFileType_None = 0x7000,
		SonyToneCurve_None = 0x7010,
		// TODO 0x800d - 0x827D
		BatteryLevel_None = 0x828F,
		KodakIFD_None = 0x8290,
		// TODO 0x82A5 - ...
		FlashEnergy_None = 0x920B,
		ExposureIndex_None = 0x9215,
		ExifInteroperabilityOffset_None = 0xA005,
		SpatialFrequencyResponse_None = 0xA20C,

		// Interop IFD
		InteropIndex_Interop = 0x0001,
		InteropVersion_Interop = 0x0002,
		RelatedImageFileFormat_Interop = 0x1000,
		RelatedImageWidth_Interop = 0x1001,
		RelatedImageHeight_Interop = 0x1002,

		// Main image IFD (IFD0)
		ProcessingSoftware_MainImage = 0x000B,
		SubfileType_MainImage = 0x00FE,
		OldSubfileType_MainImage = 0x00FF,
		ImageWidth_MainImage = 0x0100,
		ImageHeight_MainImage = 0x0101,
		BitsPerSample_MainImage = 0x0102,
		Compression_MainImage = 0x0103,
		PhotometricInterpretation_MainImage = 0x0106,
		Thresholding_MainImage = 0x0107,
		CellWidth_MainImage = 0x0108,
		CellLength_MainImage = 0x0109,
		FillOrder_MainImage = 0x010A,
		DocumentName_MainImage = 0x010D,
		ImageDescription_MainImage = 0x010E,
		Make_MainImage = 0x010F,
		Model_MainImage = 0x0110,
		PreviewImageStart1_MainImage = 0x0111,
		Orientation_MainImage = 0x0112,
		SamplesPerPixel_MainImage = 0x0115,
		RowsPerStrip_MainImage = 0x0116,
		PreviewImageLength1_MainImage = 0x0117,
		MinSampleValue_MainImage = 0x0118,
		MaxSampleValue_MainImage = 0x0119,
		XResolution_MainImage = 0x011A,
		YResolution_MainImage = 0x011B,
		PlanarConfiguration_MainImage = 0x011C,
		PageName_MainImage = 0x011D,
		XPosition_MainImage = 0x011E,
		YPosition_MainImage = 0x011F,
		GrayResponseUnit_MainImage = 0x0122,
		ResolutionUnit_MainImage = 0x0128,
		PageNumber_MainImage = 0x0129,
		TransferFunction_MainImage = 0x012D,
		Software_MainImage = 0x0131,
		DateTime_MainImage = 0x0132,
		Artist_MainImage = 0x013B,
		HostComputer_MainImage = 0x013C,
		Predictor_MainImage = 0x013D,
		WhitePoint_MainImage = 0x013E,
		PrimaryChromaticities_MainImage = 0x013F,
		HalftoneHints_MainImage = 0x0141,
		TileWidth_MainImage = 0x0142,
		TileLength_MainImage = 0x0143,
		A100DataOffset_MainImage = 0x014A,
		InkSet_MainImage = 0x014C,
		TargetPrinter_MainImage = 0x0151,
		PreviewImageStart2_MainImage = 0x0201,
		PreviewImageLength2_MainImage = 0x0202,
		YCbCrCoefficients_MainImage = 0x0211,
		YCbCrSubSampling_MainImage = 0x0212,
		YCbCrPositioning_MainImage = 0x0213,
		ReferenceBlackWhite_MainImage = 0x0214,
		ApplicationNotes_MainImage = 0x02BC,
		Rating_MainImage = 0x4746,
		RatingPercent_MainImage = 0x4749,
		Copyright_MainImage = 0x8298,
		PixelScale_MainImage = 0x830E,
		IPTC_NAA_MainImage = 0x83BB,
		IntergraphMatrix_MainImage = 0x8480,
		ModelTiePoint_MainImage = 0x8482,
		SEMInfo_MainImage = 0x8546,
		ModelTransform_MainImage = 0x85D8,
		WB_GRGBLevels_MainImage = 0x8602,
		PhotoshopSettings_MainImage = 0x8649,
		EXIFOffset_MainImage = 0x8769,
		ICC_Profile_MainImage = 0x8773,
		GeoTiffDirectory_MainImage = 0x87AF,
		GeoTiffDoubleParams_MainImage = 0x87B0,
		GeoTiffAsciiParams_MainImage = 0x87B1,
		GPSInfo_MainImage = 0x8825,
		ImageSourceData_MainImage = 0x935C,
		XPTitle_MainImage = 0x9C9B,
		XPComment_MainImage = 0x9C9C,
		XPAuthor_MainImage = 0x9C9D,
		XPKeywords_MainImage = 0x9C9E,
		XPSubject_MainImage = 0x9C9F,
		GDALMetadata_MainImage = 0xA480,
		GDALNoData_MainImage = 0xA462,
		PrintIM_MainImage = 0xC4A5,
		DNGVersion_MainImage = 0xC612,
		DNGBackwardVersion_MainImage = 0xC613,

		// Sub IFD
		SampleFormat_SubIFD = 0x0153,
		ThumbnailOffset_SubIFD = 0x0201,
		ThumbnailLength_SubIFD = 0x0202,
		VignettingCorrection_SubIFD = 0x7031,
		VignettingCorrParams_SubIFD = 0x7032,
		ChromaticAberrationCorrection_SubIFD = 0x7034,
		ChromaticAberrationCorrParams_SubIFD = 0x7035,
		DistortionCorrection_SubIFD = 0x7036,
		DistortionCorrParams_SubIFD = 0x7037,
		SonyRawImageSize_SubIFD = 0x7038,
		BlackLevel_SubIFD = 0x7310,
		WB_RGGBLevels_SubIFD = 0x7313,
		SonyCropTopLeft_SubIFD = 0x74C7,
		SonyCropSize_SubIFD = 0x74C8,
		CFARepeatPatternDim_SubIFD = 0x828D,
		CFAPattern2_SubIFD = 0x828E,

		// Thumbnail IFD
		ImageWidth_Thumbnail = 0x0100,
		ImageLength_Thumbnail = 0x0101,
		BitsPerSample_Thumbnail = 0x0102,
		Compression_Thumbnail = 0x0103,
		PhotometricInterpretation_Thumbnail = 0x0106,
		StripOffsets_Thumbnail = 0x0111,
		SamplesPerPixel_Thumbnail = 0x0115,
		RowsPerStrip_Thumbnail = 0x0116,
		StripByteConunts_Thumbnail = 0x0117,
		XResolution_Thumbnail = 0x011A,
		YResolution_Thumbnail = 0x011B,
		PlanarConfiguration_Thumbnail = 0x011C,
		ResolutionUnit_Thumbnail = 0x0128,
		JpegIFOffset_Thumbnail = 0x0201,
		JpegIFByteCount_Thumbnail = 0x0202,
		YCbCrCoefficients_Thumbnail = 0x0211,
		YCbCrSubSampling_Thumbnail = 0x0212,
		YCbCrPositioning_Thumbnail = 0x0213,
		ReferenceBlackWhite_Thumbnail = 0x0214,

		// Misc / EXIF IFD
		NewSubfileType_Misc = 0x00FE,
		SubfileType_Misc = 0x00FF,
		TransferFunction_Misc = 0x012D,
		Artist_Misc = 0x013B,
		Predictor_Misc = 0x013D,
		TileWidth_Misc = 0x0142,
		TileLength_Misc = 0x0143,
		TileOffsets_Misc = 0x0144,
		TileByteCounts_Misc = 0x0145,
		SubIFDs_Misc = 0x014A,
		JPEGTables_Misc = 0x015B,
		CFARepeatPatternDim_Misc = 0x828D,
		BatteryLevel_Misc = 0x828F,
		ExposureTime_Misc = 0x829A,
		FNumber_Misc = 0x829D,
		IPTC_NAA_Misc = 0x83BB,
		InterColorProfile_Misc = 0x8773,
		ExposureProgram_Misc = 0x8822,
		SpectralSensitivity_Misc = 0x8824,
		ISOSpeedRatings_Misc = 0x8827,
		OECF_Misc = 0x8828,
		Interlace_Misc = 0x8829,
		TimeZoneOffset_Misc = 0x882A,
		SelfTimerMode_Misc = 0x882B,
		SensitivityType_Misc = 0x8830,
		StandardOutputSensitivity_Misc = 0x8831,
		RecommendedExposureIndex_Misc = 0x8832,
		ISOSpeed_Misc = 0x8833,
		ISOSpeedLatitudeyyy_Misc = 0x8834,
		ISOSpeedLatitudezzz_Misc = 0x8835,
		ExifVersion_Misc = 0x9000,
		DateTimeOriginal_Misc = 0x9003,
		DateTimeDigitized_Misc = 0x9004,
		GooglePlusUploadCode_Misc = 0x9009,
		OffsetTime_Misc = 0x9010,
		OffsetTimeOriginal_Misc = 0x9011,
		OffsetTimeDigitized_Misc = 0x9012,
		ComponentConfiguration_Misc = 0x9101,
		CompressedBitsPerPixel_Misc = 0x9102,
		ShutterSpeedValue_Misc = 0x9201,
		ApertureValue_Misc = 0x9202,
		BrightnessValue_Misc = 0x9203,
		ExposureBiasValue_Misc = 0x9204,
		MaxApertureValue_Misc = 0x9205,
		SubjectDistance_Misc = 0x9206,
		MeteringMode_Misc = 0x9207,
		LightSource_Misc = 0x9208,
		Flash_Misc = 0x9209,
		FocalLength_Misc = 0x920A,
		SpatialFrequencyResponse1_Misc = 0x920C,
		Noise_Misc = 0x920D,
		ImageNumber_Misc = 0x9211,
		SecurityClassification_Misc = 0x9212,
		ImageHistory_Misc = 0x9213,
		SubjectArea_Misc = 0x9214,
		TIFF_EPStandardID_Misc = 0x9216,
		MakerNote_Misc = 0x927C,
		UserComment_Misc = 0x9286,
		SubSecTime_Misc = 0x9290,
		SubSecTimeOriginal_Misc = 0x9291,
		SubSecTimeDigitized_Misc = 0x9292,
		AmbientTemperature_Misc = 0x9400,
		Humidity_Misc = 0x9401,
		Pressure_Misc = 0x9402,
		WaterDepth_Misc = 0x9403,
		Acceleration_Misc = 0x9404,
		CameraElevationAngle_Misc = 0x9405,
		XiaomiSettings_Misc = 0x9999,
		XiaomiModel_Misc = 0x9A00,
		FlashPixVersion_Misc = 0xA000,
		ColorSpace_Misc = 0xA001,
		ExifImageWidth_Misc = 0xA002,
		ExifImageHeight_Misc = 0xA003,
		RelatedSoundFile_Misc = 0xA004,
		FlashEnergy_Misc = 0xA20B,
		FocalPlaneXResolution_Misc = 0xA20E,
		FocalPlaneYResolution_Misc = 0xA20F,
		FocalPlaneResolutionUnit_Misc = 0xA210,
		SubjectLocation_Misc = 0xA214,
		ExposureIndex_Misc = 0xA215,
		SensingMethod_Misc = 0xA217,
		FileSource_Misc = 0xA300,
		SceneType_Misc = 0xA301,
		CFAPattern_Misc = 0xA302,
		CustomRendered_Misc = 0xA401,
		ExposureMode_Misc = 0xA402,
		WhiteBalance_Misc = 0xA403,
		DigitalZoomRatio_Misc = 0xA404,
		FocalLengthIn35mmFormat_Misc = 0xA405,
		SceneCaptureType_Misc = 0xA406,
		GainControl_Misc = 0xA407,
		Contrast_Misc = 0xA408,
		Saturation_Misc = 0xA409,
		Sharpness_Misc = 0xA40A,
		SubjectDistanceRange_Misc = 0xA40C,
		ImageUniqueID_Misc = 0xA420,
		OwnerName_Misc = 0xA430,
		SerialNumber_Misc = 0xA431,
		LensInfo_Misc = 0xA432,
		LensMake_Misc = 0xA433,
		LensModel_Misc = 0xA434,
		LensSerialNumber_Misc = 0xA435,
		ImageTitle_Misc = 0xA436,
		Photographer_Misc = 0xA437,
		ImageEditor_Misc = 0xA438,
		CameraFirmware_Misc = 0xA439,
		RAWDevelopingSoftware_Misc = 0xA43A,
		ImageEditingSoftware_Misc = 0xA43B,
		MetadataEditingSoftware_Misc = 0xA43C,
		CompositeImage_Misc = 0xA460,
	};

	template <typename ByteT>
	GPARSING_API class EXIFTag
	{
	private:
		EXIFTagNumber m_id;
		EXIFDataType m_type;
		std::vector<std::vector<ByteT>> m_components;

	public:
		EXIFTag() : m_type(EXIFDataType::UNKNOWN), m_id(EXIFTagNumber::UNKNOWN) {		}

		EXIFTagNumber GetID() const { return m_id; }

		void SetID(const EXIFTagNumber _id) { m_id = _id; }

		size_t GetByteSize() const { return EXIFDataTypeSizes.at(m_type); }

		EXIFDataType GetType() const { return m_type; }

		void SetType(const EXIFDataType _type) { 
			m_type = _type;
			
			for (auto& component : m_components)
			{
				component.resize(GetByteSize());
			}
		}

		size_t GetComponentsCount() const { return m_components.size(); }

		std::vector<ByteT> GetComponentFromIndex(const size_t _index) const { return m_components[_index]; }

		std::vector<std::vector<ByteT>> GetComponents() const { return m_components; }

		void AppendComponent(const std::vector<ByteT>& _component) {
			if (_component.size() != GetByteSize())
			{
				throw std::runtime_error("Value size not data type size");
			}

			m_components.push_back(_component);
		}

		void PopComponent(std::vector<ByteT> &_back) {
			_back = m_components.back();
			PopComponent();
		}

		void PopComponent() {
			m_components.pop_back();
		}

		void ClearComponents() {
			m_components.clear();
		}

		template<typename CastT>
		std::vector<CastT> CastTo() const {
			std::vector<CastT> output;

			switch (GetType())
			{
			case GParsing::EXIFDataType::UNKNOWN:
				throw std::runtime_error("Unknown type, cannot cast");
				break;
			case GParsing::EXIFDataType::UBYTE:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, uint8_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::ASCII:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, uint8_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::USHORT:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, uint16_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::ULONG:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, uint32_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::URATIONAL:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					CastT numerator = BytesFromBuffer<ByteT, uint32_t>(GetComponentFromIndex(i), 0);
					CastT denominator = BytesFromBuffer<ByteT, uint32_t>(GetComponentFromIndex(i), 4);

					output.push_back(numerator / denominator);
				}
				break;
			case GParsing::EXIFDataType::BYTE:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, int8_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::UNDEFINED:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, uint8_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::SHORT:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, int16_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::LONG:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(BytesFromBuffer<ByteT, int32_t>(GetComponentFromIndex(i), 0));
				}
				break;
			case GParsing::EXIFDataType::RATIONAL:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					CastT numerator = BytesFromBuffer<ByteT, int32_t>(GetComponentFromIndex(i), 0);
					CastT denominator = BytesFromBuffer<ByteT, int32_t>(GetComponentFromIndex(i), 4);

					output.push_back(numerator / denominator);
				}
				break;
			case GParsing::EXIFDataType::FLOAT:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(static_cast<float>(BytesFromBuffer<ByteT, uint32_t>(GetComponentFromIndex(i), 0)));
				}
				break;
			case GParsing::EXIFDataType::DOUBLE:
				for (size_t i = 0; i < GetComponentsCount(); i++)
				{
					output.push_back(static_cast<double>(BytesFromBuffer<ByteT, uint64_t>(GetComponentFromIndex(i), 0)));
				}
				break;
			}

			return output;
		}
	};
}
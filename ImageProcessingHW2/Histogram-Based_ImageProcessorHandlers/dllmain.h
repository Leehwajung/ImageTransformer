// dllmain.h : 모듈 클래스의 선언입니다.

class CHistogramBased_ImageProcessorHandlersModule : public ATL::CAtlDllModuleT< CHistogramBased_ImageProcessorHandlersModule >
{
public :
	DECLARE_LIBID(LIBID_Histogram-Based_ImageProcessorHandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HISTOGRAMBASED_IMAGEPROCESSORHANDLERS, "{3ACBC6FF-7DC4-42ED-A10E-C40D85ACAEFB}")
};

extern class CHistogramBased_ImageProcessorHandlersModule _AtlModule;

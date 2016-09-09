#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

#include "..\asmlib\asmlib.h"

extern int g_VFVAPIVersion;

extern "C" int IInstrSet;

class JPSDR_ChromaShiftData
{
public:
	uint16_t shift_Cb,shift_Cr,shift2_Cb,shift2_Cr;
	bool enable_shift_Cb,enable_shift_Cr,shift_left_Cb,shift_left_Cr;
	bool enable_shift2_Cb,enable_shift2_Cr,shift_up_Cb,shift_up_Cr;

	JPSDR_ChromaShiftData(void);
};


JPSDR_ChromaShiftData::JPSDR_ChromaShiftData(void)
{
	shift_Cb=7;
	shift_Cr=7;
	enable_shift_Cb=true;
	enable_shift_Cr=true;
	shift_left_Cb=true;
	shift_left_Cr=true;

	shift2_Cb=0;
	shift2_Cr=0;
	enable_shift2_Cb=false;
	enable_shift2_Cr=false;
	shift_up_Cb=true;
	shift_up_Cr=true;
}


class JPSDR_ChromaShiftDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_ChromaShiftDialog(JPSDR_ChromaShiftData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_CHROMA_SHIFT),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_ChromaShiftData& mData;
	JPSDR_ChromaShiftData moldData;
	IVDXFilterPreview2 *const mifp;
};



INT_PTR JPSDR_ChromaShiftDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG : return !OnInit();
		case WM_DESTROY :
			OnDestroy();
			break;
		case WM_COMMAND :
			if (OnCommand(LOWORD(wParam))) return TRUE;
			break;
	}
	return FALSE;
}



bool JPSDR_ChromaShiftDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_ENABLE_Cr,mData.enable_shift_Cr?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_ENABLE_Cb,mData.enable_shift_Cb?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_ENABLE2_Cr,mData.enable_shift2_Cr?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_ENABLE2_Cb,mData.enable_shift2_Cb?BST_CHECKED:BST_UNCHECKED);

	if (mData.shift_left_Cr) CheckDlgButton(mhdlg,IDC_LEFT_Cr,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_RIGHT_Cr,BST_CHECKED);
	if (mData.shift_left_Cb) CheckDlgButton(mhdlg,IDC_LEFT_Cb,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_RIGHT_Cb,BST_CHECKED);

	if (mData.shift_up_Cr) CheckDlgButton(mhdlg,IDC_UP_Cr,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_DOWN_Cr,BST_CHECKED);
	if (mData.shift_up_Cb) CheckDlgButton(mhdlg,IDC_UP_Cb,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_DOWN_Cb,BST_CHECKED);

	SetDlgItemInt(mhdlg,IDC_SHIFT_Cr,mData.shift_Cr,FALSE);
	SetDlgItemInt(mhdlg,IDC_SHIFT_Cb,mData.shift_Cb,FALSE);
	SetDlgItemInt(mhdlg,IDC_SHIFT2_Cr,mData.shift2_Cr,FALSE);
	SetDlgItemInt(mhdlg,IDC_SHIFT2_Cb,mData.shift2_Cb,FALSE);

	EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_Cr),mData.enable_shift_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_LEFT_Cr),mData.enable_shift_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_RIGHT_Cr),mData.enable_shift_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION_Cr),mData.enable_shift_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_Cr),mData.enable_shift_Cr);

	EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_Cb),mData.enable_shift_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_LEFT_Cb),mData.enable_shift_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_RIGHT_Cb),mData.enable_shift_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION_Cb),mData.enable_shift_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_Cb),mData.enable_shift_Cb);

	EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT2_Cr),mData.enable_shift2_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_UP_Cr),mData.enable_shift2_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_DOWN_Cr),mData.enable_shift2_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION2_Cr),mData.enable_shift2_Cr);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT2_Cr),mData.enable_shift2_Cr);

	EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT2_Cb),mData.enable_shift2_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_UP_Cb),mData.enable_shift2_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_DOWN_Cb),mData.enable_shift2_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION2_Cb),mData.enable_shift2_Cb);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT2_Cb),mData.enable_shift2_Cb);

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}


void JPSDR_ChromaShiftDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_ChromaShiftDialog::SaveToData()
{
	uint16_t shift;
	BOOL success;

	mData.enable_shift_Cb=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_Cb);
	if (mData.enable_shift_Cb)
	{
		mData.shift_left_Cb=!!IsDlgButtonChecked(mhdlg,IDC_LEFT_Cb);
		shift=GetDlgItemInt(mhdlg,IDC_SHIFT_Cb,&success,FALSE);
		if (success) mData.shift_Cb=shift;
	}

	mData.enable_shift_Cr=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_Cr);
	if (mData.enable_shift_Cr)
	{
		mData.shift_left_Cr=!!IsDlgButtonChecked(mhdlg,IDC_LEFT_Cr);
		shift=GetDlgItemInt(mhdlg,IDC_SHIFT_Cr,&success,FALSE);
		if (success) mData.shift_Cr=shift;
	}

	mData.enable_shift2_Cb=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE2_Cb);
	if (mData.enable_shift2_Cb)
	{
		mData.shift_up_Cb=!!IsDlgButtonChecked(mhdlg,IDC_UP_Cb);
		shift=GetDlgItemInt(mhdlg,IDC_SHIFT2_Cb,&success,FALSE);
		if (success) mData.shift2_Cb=shift;
	}

	mData.enable_shift2_Cr=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE2_Cr);
	if (mData.enable_shift2_Cr)
	{
		mData.shift_up_Cr=!!IsDlgButtonChecked(mhdlg,IDC_UP_Cr);
		shift=GetDlgItemInt(mhdlg,IDC_SHIFT2_Cr,&success,FALSE);
		if (success) mData.shift2_Cr=shift;
	}

	return true;
}



bool JPSDR_ChromaShiftDialog::OnCommand(int cmd)
{
	switch(cmd)
	{
		case IDOK :
			SaveToData();
			EndDialog(mhdlg,true);
			return true;
		case IDCANCEL :
			mData=moldData;
			EndDialog(mhdlg,false);
			return true;
		case IDC_ENABLE_Cb :
			mData.enable_shift_Cb=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_Cb),mData.enable_shift_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_LEFT_Cb),mData.enable_shift_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_RIGHT_Cb),mData.enable_shift_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION_Cb),mData.enable_shift_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_Cb),mData.enable_shift_Cb);
			if (mifp) mifp->RedoFrame();
			return true;
		case IDC_ENABLE_Cr :
			mData.enable_shift_Cr=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_Cr),mData.enable_shift_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_LEFT_Cr),mData.enable_shift_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_RIGHT_Cr),mData.enable_shift_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION_Cr),mData.enable_shift_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_Cr),mData.enable_shift_Cr);
			if (mifp) mifp->RedoFrame();
			return true;
		case IDC_ENABLE2_Cb :
			mData.enable_shift2_Cb=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE2_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT2_Cb),mData.enable_shift2_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_UP_Cb),mData.enable_shift2_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_DOWN_Cb),mData.enable_shift2_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION2_Cb),mData.enable_shift2_Cb);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT2_Cb),mData.enable_shift2_Cb);
			if (mifp) mifp->RedoFrame();
			return true;
		case IDC_ENABLE2_Cr :
			mData.enable_shift2_Cr=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE2_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT2_Cr),mData.enable_shift2_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_UP_Cr),mData.enable_shift2_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_DOWN_Cr),mData.enable_shift2_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_DIRECTION2_Cr),mData.enable_shift2_Cr);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT2_Cr),mData.enable_shift2_Cr);
			if (mifp) mifp->RedoFrame();
			return true;
		case IDC_LEFT_Cb :
		case IDC_RIGHT_Cb :
		case IDC_SHIFT_Cb :
		case IDC_LEFT_Cr :
		case IDC_RIGHT_Cr :
		case IDC_SHIFT_Cr :
		case IDC_UP_Cb :
		case IDC_DOWN_Cb :
		case IDC_SHIFT2_Cb :
		case IDC_UP_Cr :
		case IDC_DOWN_Cr :
		case IDC_SHIFT2_Cr :
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



class JPSDR_ChromaShift : public VDXVideoFilter
{
public:
	virtual uint32 GetParams();
	virtual bool Init();
	virtual void Start();
	virtual void Run();
	virtual void End();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data idata;
	uint8_t *buffer_Y,*buffer_U,*buffer_V;
	int16_t lookupRGB[14][256];
	size_t CPU_Cache_Size,Cache_Setting;
	
	void ConvertRGB32toPlanarYCbCr(const void *src, uint8_t *dst_Y, uint8_t *dst_U, uint8_t *dst_V,
		ptrdiff_t src_modulo, const int32_t w, const int32_t h);
	void ConvertPlanarYCbCrtoRGB32(const uint8_t *src_Y, const uint8_t *src_U, const uint8_t *src_V,
		void *dst,ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void ConvertYUY2toPlanarYCbCr(const void *src, uint8_t *dst_Y, uint8_t *dst_U, uint8_t *dst_V,
		ptrdiff_t src_modulo, const int32_t w, const int32_t h);
	void ConvertPlanarYCbCrtoYUY2(const uint8_t *src_Y, const uint8_t *src_U, const uint8_t *src_V,
		void *dst,ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void ConvertUYVYtoPlanarYCbCr(const void *src, uint8_t *dst_Y, uint8_t *dst_U, uint8_t *dst_V,
		ptrdiff_t src_modulo, const int32_t w, const int32_t h);
	void ConvertPlanarYCbCrtoUYVY(const uint8_t *src_Y, const uint8_t *src_U, const uint8_t *src_V,
		void *dst,ptrdiff_t dst_modulo, const int32_t w, const int32_t h);

	void ShiftPlanarLeft(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
		const int32_t w, const int32_t h, const int32_t shift,const uint8_t data);
	void ShiftPlanarRight(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
		const int32_t w, const int32_t h,const int32_t shift,const uint8_t data);
	void ShiftPlanarUp(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
		const int32_t w, const int32_t h, const int32_t shift, const uint8_t data);
	void ShiftPlanarDown(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
		const int32_t w, const int32_t h,const int32_t shift,const uint8_t data);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_ChromaShiftData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_ChromaShift)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_ChromaShift,ScriptConfig,"iiiiiiiiiiii")
VDXVF_END_SCRIPT_METHODS()


uint32 JPSDR_ChromaShift::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;
	
	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :*/				
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
//		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
//		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :			*/
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM);
}


bool JPSDR_ChromaShift::Configure(VDXHWND hwnd)
{
	JPSDR_ChromaShiftDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_ChromaShift::GetSettingString(char *buf, int maxlen)
{
	char out_string[200];

	strcpy_s(out_string,200," Cb : ");
	strcat_s(out_string,200,"%d");
	if (mData.enable_shift_Cb)
	{
		if (mData.shift_left_Cb) strcat_s(out_string,200," (L) / ");
		else strcat_s(out_string,200," (R) / ");
	} else strcat_s(out_string,200," (x) / ");
	strcat_s(out_string,200,"%d");
	if (mData.enable_shift2_Cb)
	{
		if (mData.shift_up_Cb) strcat_s(out_string,200," (U)");
		else strcat_s(out_string,200," (D)");
	} else strcat_s(out_string,200," (x) ");
	strcat_s(out_string,200," Cr : ");
	strcat_s(out_string,200,"%d");
	if (mData.enable_shift_Cr)
	{
		if (mData.shift_left_Cr) strcat_s(out_string,200," (L) / ");
		else strcat_s(out_string,200," (R) / ");
	} else strcat_s(out_string,200," (x) / ");
	strcat_s(out_string,200,"%d");
	if (mData.enable_shift2_Cr)
	{
		if (mData.shift_up_Cr) strcat_s(out_string,200," (U)");
		else strcat_s(out_string,200," (D)");
	} else strcat_s(out_string,200," (x) ");
	SafePrintf(buf,maxlen,out_string,mData.shift_Cb,mData.shift2_Cb,mData.shift_Cr,mData.shift2_Cr);
}


bool JPSDR_ChromaShift::Init()
{
	int16_t i;
	double r_y,g_y,b_y,r_u,g_u,b_u,r_v,g_v,b_v;

	r_y=0.299; g_y=0.587; b_y=0.114;
	r_u=-r_y*0.564; g_u=-g_y*0.564; b_u=(1.0-b_y)*0.564;
	r_v=(1.0-r_y)*0.713; g_v=-g_y*0.713; b_v=-b_y*0.713;
	
	for (i=0; i<256; i++)
	{
		lookupRGB[0][i]=(int16_t)round(((i*r_y*32.0*219.0)/255.0)+16.0*32.0+16.0);
		lookupRGB[1][i]=(int16_t)round((i*g_y*32.0*219.0)/255.0);
		lookupRGB[2][i]=(int16_t)round((i*b_y*32.0*219.0)/255.0);
		lookupRGB[3][i]=(int16_t)round(((i*r_u*32.0*224.0)/255.0)+128.0*32.0+16.0);
		lookupRGB[4][i]=(int16_t)round((i*g_u*32.0*224.0)/255.0);
		lookupRGB[5][i]=(int16_t)round((i*b_u*32.0*224.0)/255.0);
		lookupRGB[6][i]=(int16_t)round(((i*r_v*32.0*224.0)/255.0)+128.0*32.0+16.0);
		lookupRGB[7][i]=(int16_t)round((i*g_v*32.0*224.0)/255.0);
		lookupRGB[8][i]=(int16_t)round((i*b_v*32.0*224.0)/255.0);
		lookupRGB[9][i]=(int16_t)round((i-16.0)*1.164*32.0+16.0);
		lookupRGB[10][i]=(int16_t)round((i-128.0)*1.596*32.0);
		lookupRGB[11][i]=(int16_t)round((i-128.0)*-0.813*32.0);
		lookupRGB[12][i]=(int16_t)round((i-128.0)*-0.391*32.0);
		lookupRGB[13][i]=(int16_t)round((i-128.0)*2.018*32.0);
	}
	buffer_Y=NULL;
	buffer_U=NULL;
	buffer_V=NULL;

	if (IInstrSet<0) InstructionSet();
	CPU_Cache_Size=DataCacheSize(0)>>2;

	return(true);
}


void JPSDR_ChromaShift::ConvertYUY2toPlanarYCbCr(const void *_src, uint8_t *dst_Y, uint8_t *dst_U, uint8_t *dst_V,
	ptrdiff_t src_modulo, const int32_t w, const int32_t h)
{
	const int32_t w_2=(w+1)>>1;
	const YUYV *src;

	src=(YUYV*)_src;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w_2; j++)
		{
			*dst_Y++=src->y1;
			*dst_Y++=src->y2;
			*dst_U++=src->u;
			*dst_V++=src->v;
			src++;
		}
		src=(YUYV*)((uint8_t*)src+src_modulo);
	}
}


void JPSDR_ChromaShift::ConvertUYVYtoPlanarYCbCr(const void *_src, uint8_t *dst_Y, uint8_t *dst_U, uint8_t *dst_V,
	ptrdiff_t src_modulo, const int32_t w, const int32_t h)
{
	const int32_t w_2=(w+1)>>1;
	const UYVY *src;

	src=(UYVY*)_src;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w_2; j++)
		{
			*dst_Y++=src->y1;
			*dst_Y++=src->y2;
			*dst_U++=src->u;
			*dst_V++=src->v;
			src++;
		}
		src=(UYVY*)((uint8_t*)src+src_modulo);
	}
}



void JPSDR_ChromaShift::ConvertPlanarYCbCrtoYUY2(const uint8_t *src_Y, const uint8_t *src_U, const uint8_t *src_V,
	void *_dst, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const int32_t w_2=(w+1)>>1;
	YUYV *dst;

	dst=(YUYV*)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w_2; j++)
		{
			dst->y1=*src_Y++;
			dst->y2=*src_Y++;
			dst->u=*src_U++;
			dst->v=*src_V++;
			dst++;
		}
		dst=(YUYV*)((uint8_t*)dst+dst_modulo);
	}
}



void JPSDR_ChromaShift::ConvertPlanarYCbCrtoUYVY(const uint8_t *src_Y, const uint8_t *src_U, const uint8_t *src_V,
	void *_dst, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const int32_t w_2=(w+1)>>1;
	UYVY *dst;

	dst=(UYVY*)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w_2; j++)
		{
			dst->y1=*src_Y++;
			dst->y2=*src_Y++;
			dst->u=*src_U++;
			dst->v=*src_V++;
			dst++;
		}
		dst=(UYVY*)((uint8_t*)dst+dst_modulo);
	}
}


void JPSDR_ChromaShift::ConvertRGB32toPlanarYCbCr(const void *_src, uint8_t *dst_Y, uint8_t *dst_U, uint8_t *dst_V,
	ptrdiff_t src_modulo, const int32_t w, const int32_t h)
{
	const RGB32BMP* src;
	uint8_t r,g,b;

	src=(RGB32BMP*)_src;

	for (int32_t i=0; i<h; i++)
	{
		for(int32_t j=0; j<w; j++)
		{
			r=src->r;
			g=src->g;
			b=src->b;
			src++;
			*dst_Y++=(uint8_t)((lookupRGB[0][r]+lookupRGB[1][g]+lookupRGB[2][b])>>5);
			*dst_U++=(uint8_t)((lookupRGB[3][r]+lookupRGB[4][g]+lookupRGB[5][b])>>5);
			*dst_V++=(uint8_t)((lookupRGB[6][r]+lookupRGB[7][g]+lookupRGB[8][b])>>5);
		}
		src=(RGB32BMP*)((uint8_t*)src+src_modulo);
	}
}


void JPSDR_ChromaShift::ConvertPlanarYCbCrtoRGB32(const uint8_t *src_Y, const uint8_t *src_U, const uint8_t *src_V,
	void *_dst,ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	RGB32BMP *dst;
	int16_t y,calcul;

	dst=(RGB32BMP*)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			y=lookupRGB[9][*src_Y++];
			calcul=(y+lookupRGB[10][*src_V])>>5;
			if (calcul<0) calcul=0;
			else
			{
				if (calcul>255) calcul=255;
			}
			dst->r=(uint8_t)calcul;
			calcul=(y+lookupRGB[11][*src_V++]+lookupRGB[12][*src_U])>>5;
			if (calcul<0) calcul=0;
			else
			{
				if (calcul>255) calcul=255;
			}
			dst->g=(uint8_t)calcul;
			calcul=(y+lookupRGB[13][*src_U++])>>5;
			if (calcul<0) calcul=0;
			else
			{
				if (calcul>255) calcul=255;
			}
			dst->b=(uint8_t)calcul;
			dst->alpha=0;
			dst++;
		}
		dst=(RGB32BMP*)((uint8_t*)dst+dst_modulo);
	}
}




void JPSDR_ChromaShift::ShiftPlanarLeft(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
	const int32_t w, const int32_t h,const int32_t shift, const uint8_t data)
{
	if ((shift==0) || (shift>=w)) return;

	const int32_t w_shift=w-shift;
	const uint8_t *src=(uint8_t *)src_+shift;
	uint8_t *dst=(uint8_t *)dst_;
	uint8_t *dst_a=dst+w_shift;

	for (int32_t i=0; i<h; i++)
	{
		A_memmove(dst,src,w_shift);
		A_memset(dst_a,data,shift);
		src+=src_pitch;
		dst+=dst_pitch;
		dst_a+=dst_pitch;
	}
}


void JPSDR_ChromaShift::ShiftPlanarRight(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
	const int32_t w, const int32_t h,const int32_t shift, const uint8_t data)
{
	if ((shift==0) || (shift>=w)) return;

	const uint8_t *src=(uint8_t *)src_;
	const int32_t w_shift=w-shift;
	uint8_t *dst_b=(uint8_t *)dst_;
	uint8_t *dst_a=dst_b+shift;

	for (int32_t i=0; i<h; i++)
	{
		A_memmove(dst_a,src,w_shift);
		A_memset(dst_b,data,shift);
		src+=src_pitch;
		dst_a+=dst_pitch;
		dst_b+=dst_pitch;
	}
}



void JPSDR_ChromaShift::ShiftPlanarUp(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
	const int32_t w, const int32_t h, const int32_t shift, const uint8_t data)
{
	if ((shift==0) || (shift>=h)) return;

	const uint8_t *src=(uint8_t *)src_+shift*src_pitch;
	uint8_t *dst=(uint8_t *)dst_;

	if ((src_pitch==dst_pitch) && (src_pitch==w))
	{
		A_memcpy(dst,src,(size_t)w*(size_t)(h-shift));
		dst+=(h-shift)*w;

		A_memset(dst,data,(size_t)w*(size_t)shift);
	}
	else
	{
		for (int32_t i=shift; i<h; i++)
		{
			A_memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}

		for (int32_t i=0; i<shift; i++)
		{
			A_memset(dst,data,w);
			dst+=dst_pitch;			
		}
	}
}


void JPSDR_ChromaShift::ShiftPlanarDown(const void *src_, void *dst_, ptrdiff_t src_pitch, ptrdiff_t dst_pitch,
	const int32_t w, const int32_t h, const int32_t shift, const uint8_t data)
{
	if ((shift==0) || (shift>=h)) return;

	const uint8_t *src=(uint8_t *)src_+((h-shift-1)*src_pitch);
	uint8_t *dst=(uint8_t *)dst_+(h-1)*dst_pitch;

	if ((src_pitch==dst_pitch) && (-src_pitch==w))
	{
		A_memcpy(dst,src,(size_t)w*(size_t)(h-shift));
		dst+=(h-shift)*w;

		A_memset(dst,data,(size_t)w*(size_t)shift);
	}
	else
	{
		for (int32_t i=shift; i<h; i++)
		{
			A_memcpy(dst,src,w);
			src-=src_pitch;
			dst-=dst_pitch;
		}

		for (int32_t i=0; i<shift; i++)
		{
			A_memset(dst,data,w);
			dst-=dst_pitch;			
		}
	}
}



void JPSDR_ChromaShift::Run()
{
	uint8_t *planar_Y,*planar_U,*planar_V;
	int32_t w_Y,w_U,w_V,h_Y,h_U,h_V;
	uint16_t shift_Y,shift_U,shift_V;
	uint16_t shift2_Y,shift2_U,shift2_V;
	bool shift1Y,shift1U,shift1V;
	bool shift2Y,shift2U,shift2V;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;

	const VDXPixmap& pxdst=*fa->dst.mpPixmap;
	const VDXPixmap& pxsrc=*fa->src.mpPixmap;

	idata.src_plane0=pxsrc.data;
	idata.src_plane1=pxsrc.data2;
	idata.src_plane2=pxsrc.data3;
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	SetMemcpyCacheLimit(Cache_Setting);
	SetMemsetCacheLimit(Cache_Setting);

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
		case 2 :
		case 3 :
			planar_Y=buffer_Y;
			planar_U=buffer_U;
			planar_V=buffer_V;
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
			planar_Y=(uint8_t *)idata.src_plane0;
			planar_U=(uint8_t *)idata.src_plane1;
			planar_V=(uint8_t *)idata.src_plane2;
			break;
	}

	switch (idata.video_mode)
	{
		case 0 :
		case 1 :
			w_Y=idata.src_w0;
			w_U=w_Y;
			w_V=w_Y;
			h_Y=idata.src_h0;
			h_U=h_Y;
			h_V=h_Y;
			ConvertRGB32toPlanarYCbCr(idata.src_plane0,planar_Y,planar_U,planar_V,idata.src_modulo0,w_Y,h_Y);
			break;
		case 2 :
			w_Y=idata.src_w0;
			w_U=(w_Y+1)>>1;
			w_V=(w_Y+1)>>1;
			h_Y=idata.src_h0;
			h_U=h_Y;
			h_V=h_Y;
			ConvertYUY2toPlanarYCbCr(idata.src_plane0,planar_Y,planar_U,planar_V,idata.src_modulo0,w_Y,h_Y);
			break;
		case 3 :
			w_Y=idata.src_w0;
			w_U=(w_Y+1)>>1;
			w_V=(w_Y+1)>>1;
			h_Y=idata.src_h0;
			h_U=h_Y;
			h_V=h_Y;
			ConvertUYVYtoPlanarYCbCr(idata.src_plane0,planar_Y,planar_U,planar_V,idata.src_modulo0,w_Y,h_Y);
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
			w_Y=idata.src_w0;
			w_U=idata.src_w1;
			w_V=idata.src_w2;
			h_Y=idata.src_h0;
			h_U=idata.src_h1;
			h_V=idata.src_h2;
			break;
	}

	if ((mData.enable_shift_Cb) && (mData.enable_shift_Cr) && (mData.shift_left_Cb==mData.shift_left_Cr)
		&& (mData.shift_Cb==mData.shift_Cr))
	{
		shift_Y=mData.shift_Cb;
		shift_U=0;
		shift_V=0;
	}
	else
	{
		shift_Y=0;
		if (mData.enable_shift_Cb) shift_U=mData.shift_Cb;
		else shift_U=0;
		if (mData.enable_shift_Cr) shift_V=mData.shift_Cr;
		else shift_V=0;
	}

	if ((mData.enable_shift2_Cb) && (mData.enable_shift2_Cr) && (mData.shift_up_Cb==mData.shift_up_Cr)
		&& (mData.shift2_Cb==mData.shift2_Cr))
	{
		shift2_Y=mData.shift2_Cb;
		shift2_U=0;
		shift2_V=0;
	}
	else
	{
		shift2_Y=0;
		if (mData.enable_shift2_Cb) shift2_U=mData.shift2_Cb;
		else shift2_U=0;
		if (mData.enable_shift2_Cr) shift2_V=mData.shift2_Cr;
		else shift2_V=0;
	}

	switch (idata.video_mode)
	{
		case 1 :
		case 4 :
			if (shift_Y!=0)
			{
				shift_U=shift_Y;
				shift_V=shift_Y;
				shift_Y=0;
			}
			break;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
			if (shift_Y==0)
			{
				shift_U=(shift_U+1)>>1;
				shift_V=(shift_V+1)>>1;
			}
			else
			{
				shift_U=shift_Y>>1;
				shift_V=shift_Y>>1;
				shift_Y%=2;
			}
			break;
		case 7 :
		case 8 :
			if (shift_Y==0)
			{
				shift_U=(shift_U+3)>>2;
				shift_V=(shift_V+3)>>2;
			}
			else
			{
				shift_U=shift_Y>>2;
				shift_V=shift_Y>>2;
				shift_Y%=4;
			}
			break;
	}
	shift1Y=(shift_Y!=0);
	shift1U=mData.enable_shift_Cb && (shift_U!=0);
	shift1V=mData.enable_shift_Cr && (shift_V!=0);

	switch (idata.video_mode)
	{
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 7 :
			if (shift2_Y!=0)
			{
				shift2_U=shift2_Y;
				shift2_V=shift2_Y;
				shift2_Y=0;
			}
			break;
		case 6 :
			if (shift2_Y==0)
			{
				shift2_U=(shift2_U+1)>>1;
				shift2_V=(shift2_V+1)>>1;
			}
			else
			{
				shift2_U=shift2_Y>>1;
				shift2_V=shift2_Y>>1;
				shift2_Y%=2;
			}
			break;
		case 8 :
			if (shift2_Y==0)
			{
				shift2_U=(shift2_U+3)>>2;
				shift2_V=(shift2_V+3)>>2;
			}
			else
			{
				shift2_U=shift2_Y>>2;
				shift2_V=shift2_Y>>2;
				shift2_Y%=4;
			}
			break;
	}
	shift2Y=(shift2_Y!=0);
	shift2U=mData.enable_shift2_Cb && (shift2_U!=0);
	shift2V=mData.enable_shift2_Cr && (shift2_V!=0);

	if (shift1Y)
	{
		switch (idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				if (mData.shift_left_Cb) ShiftPlanarRight(planar_Y,planar_Y,w_Y,w_Y,w_Y,h_Y,shift_Y,16);
				else ShiftPlanarLeft(planar_Y,planar_Y,w_Y,w_Y,w_Y,h_Y,shift_Y,16);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (shift2Y)
				{
					if (mData.shift_left_Cb) ShiftPlanarRight(planar_Y,planar_Y,idata.src_pitch0,idata.src_pitch0,
						w_Y,h_Y,shift_Y,16);
					else ShiftPlanarLeft(planar_Y,planar_Y,idata.src_pitch0,idata.src_pitch0,w_Y,h_Y,shift_Y,16);
				}
				else
				{
					if (mData.shift_left_Cb) ShiftPlanarRight(planar_Y,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,
						w_Y,h_Y,shift_Y,16);
					else ShiftPlanarLeft(planar_Y,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,w_Y,h_Y,shift_Y,16);
				}
				break;
		}
	}
	if (shift1U)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				if (mData.shift_left_Cb) ShiftPlanarLeft(planar_U,planar_U,w_U,w_U,w_U,h_U,shift_U,128);
				else ShiftPlanarRight(planar_U,planar_U,w_U,w_U,w_U,h_U,shift_U,128);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (shift2U)
				{
					if (mData.shift_left_Cb) ShiftPlanarLeft(planar_U,planar_U,idata.src_pitch1,idata.src_pitch1,
						w_U,h_U,shift_U,128);
					else ShiftPlanarRight(planar_U,planar_U,idata.src_pitch1,idata.src_pitch1,w_U,h_U,shift_U,128);
				}
				else
				{
					if (mData.shift_left_Cb) ShiftPlanarLeft(planar_U,idata.dst_plane1,idata.src_pitch1,idata.dst_pitch1,
						w_U,h_U,shift_U,128);
					else ShiftPlanarRight(planar_U,idata.dst_plane1,idata.src_pitch1,idata.dst_pitch1,w_U,h_U,shift_U,128);
				}
				break;
		}
	}
	if (shift1V)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				if (mData.shift_left_Cr) ShiftPlanarLeft(planar_V,planar_V,w_V,w_V,w_V,h_V,shift_V,128);
				else ShiftPlanarRight(planar_V,planar_V,w_V,w_V,w_V,h_V,shift_V,128);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (shift2V)
				{
					if (mData.shift_left_Cr) ShiftPlanarLeft(planar_V,planar_V,idata.src_pitch2,idata.src_pitch2,
						w_V,h_V,shift_V,128);
					else ShiftPlanarRight(planar_V,planar_V,idata.src_pitch2,idata.src_pitch2,w_V,h_V,shift_V,128);
				}
				else
				{
					if (mData.shift_left_Cr) ShiftPlanarLeft(planar_V,idata.dst_plane2,idata.src_pitch2,idata.dst_pitch2,
						w_V,h_V,shift_V,128);
					else ShiftPlanarRight(planar_V,idata.dst_plane2,idata.src_pitch2,idata.dst_pitch2,w_V,h_V,shift_V,128);
				}
				break;
		}
	}


	if (shift2Y)
	{
		switch (idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				if (mData.shift_up_Cb) ShiftPlanarDown(planar_Y,planar_Y,w_Y,w_Y,w_Y,h_Y,shift2_Y,16);
				else ShiftPlanarUp(planar_Y,planar_Y,w_Y,w_Y,w_Y,h_Y,shift2_Y,16);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (mData.shift_up_Cb) ShiftPlanarDown(planar_Y,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,
					w_Y,h_Y,shift2_Y,16);
				else ShiftPlanarUp(planar_Y,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,w_Y,h_Y,shift2_Y,16);
				break;
		}
	}
	if (shift2U)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				if (mData.shift_up_Cb) ShiftPlanarUp(planar_U,planar_U,w_U,w_U,w_U,h_U,shift2_U,128);
				else ShiftPlanarDown(planar_U,planar_U,w_U,w_U,w_U,h_U,shift2_U,128);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (mData.shift_up_Cb) ShiftPlanarUp(planar_U,idata.dst_plane1,idata.src_pitch1,idata.dst_pitch1,
					w_U,h_U,shift2_U,128);
				else ShiftPlanarDown(planar_U,idata.dst_plane1,idata.src_pitch1,idata.dst_pitch1,w_U,h_U,shift2_U,128);
				break;
		}
	}
	if (shift2V)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				if (mData.shift_up_Cr) ShiftPlanarUp(planar_V,planar_V,w_V,w_V,w_V,h_V,shift2_V,128);
				else ShiftPlanarDown(planar_V,planar_V,w_V,w_V,w_V,h_V,shift2_V,128);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (mData.shift_up_Cr) ShiftPlanarUp(planar_V,idata.dst_plane2,idata.src_pitch2,idata.dst_pitch2,
					w_V,h_V,shift2_V,128);
				else ShiftPlanarDown(planar_V,idata.dst_plane2,idata.src_pitch2,idata.dst_pitch2,w_V,h_V,shift2_V,128);
				break;
		}
	}

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			ConvertPlanarYCbCrtoRGB32(planar_Y,planar_U,planar_V,idata.dst_plane0,idata.dst_modulo0,w_Y,h_Y);
			break;
		case 2 :
			ConvertPlanarYCbCrtoYUY2(planar_Y,planar_U,planar_V,idata.dst_plane0,idata.dst_modulo0,w_Y,h_Y);
			break;
		case 3 :
			ConvertPlanarYCbCrtoUYVY(planar_Y,planar_U,planar_V,idata.dst_plane0,idata.dst_modulo0,w_Y,h_Y);
			break;
	}
}




void JPSDR_ChromaShift::Start()
{
	bool ok;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter !");
		return;
	}
	
	const VDXPixmapLayout& pxdst=*fa->dst.mpPixmapLayout;
	const VDXPixmapLayout& pxsrc=*fa->src.mpPixmapLayout;

		idata.src_h0=pxsrc.h;
		idata.src_w0=pxsrc.w;
		idata.src_pitch0=pxsrc.pitch;
		idata.src_pitch1=pxsrc.pitch2;
		idata.src_pitch2=pxsrc.pitch3;

		idata.dst_h0=pxdst.h;
		idata.dst_w0=pxdst.w;
		idata.dst_pitch0=pxdst.pitch;
		idata.dst_pitch1=pxdst.pitch2;
		idata.dst_pitch2=pxdst.pitch3;
		
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.src_video_mode=1;
				idata.video_mode=1;
				idata.src_modulo0=idata.src_pitch0-4*idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :			
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;																
				break;
		}
		switch(pxdst.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.dst_video_mode=1;
				idata.dst_modulo0=idata.dst_pitch0-4*idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :			
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :			
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :			
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :			
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :			
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :			
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :			
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :						
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
		}

	switch (idata.video_mode)
	{
		case 0 :
		case 1 :
			buffer_Y=(uint8_t *)malloc(idata.src_w0*idata.src_h0);
			buffer_U=(uint8_t *)malloc(idata.src_w0*idata.src_h0);
			buffer_V=(uint8_t *)malloc(idata.src_w0*idata.src_h0);
			break;
		case 2 :
		case 3 :
			buffer_Y=(uint8_t *)malloc(idata.src_w0*idata.src_h0);
			buffer_U=(uint8_t *)malloc(((idata.src_w0+1)>>1)*idata.src_h0);
			buffer_V=(uint8_t *)malloc(((idata.src_w0+1)>>1)*idata.src_h0);
			break;
	}

	ok=true;
	switch (idata.video_mode)
	{
		case 0 :
		case 1 :
		case 2 :
		case 3 :
			ok=ok && (buffer_Y!=NULL);
			ok=ok && (buffer_U!=NULL);
			ok=ok && (buffer_V!=NULL);
			break;
	}

	if (!ok)
	{
		myfree(buffer_V);
		myfree(buffer_U);
		myfree(buffer_Y);
		ff->ExceptOutOfMemory();
	}

	const size_t img_size=idata.dst_size0+idata.dst_size1+idata.dst_size2;

	if (img_size<=MAX_CACHE_SIZE)
	{
		if (CPU_Cache_Size>=img_size) Cache_Setting=img_size;
		else Cache_Setting=16;
	}
	else Cache_Setting=16;

}



void JPSDR_ChromaShift::End()
{
	myfree(buffer_V);
	myfree(buffer_U);
	myfree(buffer_Y);
}



void JPSDR_ChromaShift::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{

	mData.enable_shift_Cb=!!argv[0].asInt();
	mData.shift_left_Cb=!!argv[1].asInt();
	mData.shift_Cb=argv[2].asInt();
	mData.enable_shift_Cr=!!argv[3].asInt();
	mData.shift_left_Cr=!!argv[4].asInt();
	mData.shift_Cr=argv[5].asInt();

	mData.enable_shift2_Cb=!!argv[6].asInt();
	mData.shift_up_Cb=!!argv[7].asInt();
	mData.shift2_Cb=argv[8].asInt();
	mData.enable_shift2_Cr=!!argv[9].asInt();
	mData.shift_up_Cr=!!argv[10].asInt();
	mData.shift2_Cr=argv[11].asInt();
}



void JPSDR_ChromaShift::GetScriptString(char *buf, int maxlen)
{
	SafePrintf(buf,maxlen,"Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",mData.enable_shift_Cb,mData.shift_left_Cb,
		mData.shift_Cb,mData.enable_shift_Cr,mData.shift_left_Cr,mData.shift_Cr,mData.enable_shift2_Cb,mData.shift_up_Cb,
		mData.shift2_Cb,mData.enable_shift2_Cr,mData.shift_up_Cr,mData.shift2_Cr);
}


extern VDXFilterDefinition filterDef_JPSDR_ChromaShift=
VDXVideoFilterDefinition<JPSDR_ChromaShift>("JPSDR","ChromaShift v2.1.0","Shift chrominace channels");


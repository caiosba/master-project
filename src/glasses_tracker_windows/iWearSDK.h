// File Name:	iWearSDK.h		
// Description:	Function prototypes for iWear tracking and Stereoscopy support.
//				Provides means of loading the IWEARDRV.dll and IWRSTDRV.dll support drivers explicitly into applications process.
#ifndef _IWR_EXPLICIT_SUPPORT
#define _IWR_EXPLICIT_SUPPORT
#include <windows.h>
#include <tchar.h>
#define IWEARDRV_EXPLICIT
#include "iweardrv.h"
#define IWRSTDRV_EXPLICIT
#include "iwrstdrv.h"
#ifdef __cplusplus
extern "C" {
#endif
// StereoModes
enum { IWR_MONO_MODE = 0, IWR_STEREO_MODE = 1 };
//-----------------------------------------------------------------------------
// Useful Conversion constants
//-----------------------------------------------------------------------------
#define IWR_PI					3.14159265358979f
#define IWR_RAWTODEG			180.0f / 32768.0f
#define IWR_RAWTORAD			IWR_PI / 32768.0f
// Function return defines.
enum IWR_RETURNS {
	IWR_OK					= ERROR_SUCCESS,				// 0= Tracker driver interface installed and connected ok.
	IWR_NO_TRACKER			= ERROR_FILE_NOT_FOUND,			// 1= Tracker Driver is not installed.
	IWR_NO_STEREO			= ERROR_OPEN_FAILED,			// 5= Stereo driver not installed.
	IWR_OFFLINE				= ERROR_DEV_NOT_EXIST,			// 2= Tracker driver interface is installed, yet appears to be offline or not responding.
	IWR_TRACKER_CORRUPT		= ERROR_INVALID_FUNCTION,		// 3= Tracker driver installed; however, there are missing exports.
	IWR_STEREO_CORRUPT		= ERROR_INVALID_DATA,			// 6= Stereo driver installed; however, there are missing exports.
	IWR_NOTRACKER_INSTANCE	= ERROR_INVALID_HANDLE			// 4= Attempt to free the tracker driver; however, the tracker driver did not previously load successfully.
};
// Function prototyping.
#ifdef IWEAR_ONETIME_DEFINITIONS
// Name:	IWRLoadDll
// Desc:	loading the iWear tracker driver and initializes the tracking system
// Returns:	IWR_OK, IWR_NO_TRACKER, IWR_TRACKER_CORRUPT, IWR_NO_STEREO, IWR_STEREO_CORRUPT
//			Performs onetime IWROpenTracker(...) to initiate tracker connection, return codes ignored
long	IWRLoadDll(void){
	// Tracker/interface subsystem support.
	IWRTrackerInstance = LoadLibrary( _T("IWEARDRV.DLL") );
	if( IWRTrackerInstance == NULL) 
		return IWR_NO_TRACKER;
	IWROpenTracker			= (PIWROPENTRACKER)			GetProcAddress(IWRTrackerInstance, "IWROpenTracker" );
	IWRCloseTracker			= (PIWRCLOSETRACKER)		GetProcAddress(IWRTrackerInstance, "IWRCloseTracker" );
	IWRZeroSet				= (PIWRZEROSET)				GetProcAddress(IWRTrackerInstance, "IWRZeroSet" );
	IWRGetTracking			= (PIWRGETTRACKING)			GetProcAddress(IWRTrackerInstance, "IWRGetTracking" );
	IWRGet6DTracking		= (PIWRGET6DTRACKING)		GetProcAddress(IWRTrackerInstance, "IWRGet6DTracking" );
	IWRGetFilteredSensorData= (PIWRGETFILTEREDSENSORDATA)GetProcAddress(IWRTrackerInstance,"IWRGetFilteredSensorData" );
	IWRGetSensorData		= (PIWRGETSENSORDATA)		GetProcAddress(IWRTrackerInstance, "IWRGetSensorData" );
	IWRBeginCalibrate		= (PIWRBEGINCALIBRATE)		GetProcAddress(IWRTrackerInstance, "IWRBeginCalibrate" );
	IWREndCalibrate			= (PIWRENDCALIBRATE)		GetProcAddress(IWRTrackerInstance, "IWREndCalibrate" );
	IWRGetFilterState		= (PIWRGETFILTERSTATE)		GetProcAddress(IWRTrackerInstance, "IWRGetFilterState" );
	IWRSetFilterState		= (PIWRSETFILTERSTATE)		GetProcAddress(IWRTrackerInstance, "IWRSetFilterState" );
	IWRGetProductID			= (PIWRGETPRODUCTID)		GetProcAddress(IWRTrackerInstance, "IWRGetProductID" );
	IWRGetProductDetails	= (PIWRGETPRODUCTDETAILS)	GetProcAddress(IWRTrackerInstance, "IWRGetProductDetails" );
	IWRGetVersion			= (PIWRGETVERSION)			GetProcAddress(IWRTrackerInstance, "IWRGetVersion" );
	IWRSetMagAutoCorrect	= (PIWRSETMAGAUTOCORRECT)	GetProcAddress(IWRTrackerInstance, "IWRSetMagAutoCorrect" );
	IWRGetMagYaw			= (PIWRGETMAGYAW)			GetProcAddress(IWRTrackerInstance, "IWRGetMagYaw" );
	if( !IWROpenTracker || !IWRCloseTracker || !IWRZeroSet || !IWRGetTracking || !IWRGet6DTracking || !IWRBeginCalibrate ||
		!IWRGetFilteredSensorData || !IWRGetSensorData || !IWRGetFilterState || !IWRSetFilterState || !IWREndCalibrate ||
		!IWRGetProductID || !IWRGetProductDetails || !IWRGetVersion || !IWRSetMagAutoCorrect || !IWRGetMagYaw )
		return IWR_TRACKER_CORRUPT;
	// Stereo/video subsystem support.
	IWRStereoInstance = LoadLibrary( _T("IWRSTDRV.DLL") );
	if( IWRStereoInstance == NULL) 
		return IWR_NO_STEREO;
	IWRSTEREO_Open			= (PIWRSTEREOOPEN)			GetProcAddress(IWRStereoInstance, "IWRSTEREO_Open" );
	IWRSTEREO_Close			= (PIWRSTEREOCLOSE)			GetProcAddress(IWRStereoInstance, "IWRSTEREO_Close" );
	IWRSTEREO_SetStereo		= (PIWRSTEREOSETSTEREO)		GetProcAddress(IWRStereoInstance, "IWRSTEREO_SetStereo" );
	IWRSTEREO_WaitForAck	= (PIWRSTEREOWACK)			GetProcAddress(IWRStereoInstance, "IWRSTEREO_WaitForAck" );
	IWRSTEREO_SetLR			= (PIWRSTEREOSETLR)			GetProcAddress(IWRStereoInstance, "IWRSTEREO_SetLR" );
	IWRSTEREO_GetVersion	= (PIWRSTEREOGETVERSION)	GetProcAddress(IWRStereoInstance, "IWRSTEREO_GetVersion" );
	if( !IWRSTEREO_Open || !IWRSTEREO_Close || !IWRSTEREO_SetStereo || !IWRSTEREO_WaitForAck || !IWRSTEREO_SetLR || !IWRSTEREO_GetVersion )
		return IWR_STEREO_CORRUPT;
	IWROpenTracker();
	return IWR_OK;
}
// Name:	IWRFreeDll
// Desc:	Frees the dynamically loaded tracker/stereo driver.
// Retu:    IWR_OK, IWR_NO_INSTANCE
long	IWRFreeDll( void ){
	if( IWRStereoInstance ) {
		FreeLibrary(IWRStereoInstance);
		IWRStereoInstance = NULL;
	}
	if( IWRTrackerInstance ) {
		if( IWRCloseTracker )
			IWRCloseTracker();	
		FreeLibrary( IWRTrackerInstance );
		IWRTrackerInstance = NULL;
		return IWR_OK;		
	}
	return IWR_NOTRACKER_INSTANCE;
}
#else
extern long IWRLoadDll( void );
extern long IWRFreeDll( void );
#endif // IWEAR_ONETIME_DEFINITIONS
#ifdef __cplusplus
}
#endif
#endif //_IWR_EXPLICIT_SUPPORT
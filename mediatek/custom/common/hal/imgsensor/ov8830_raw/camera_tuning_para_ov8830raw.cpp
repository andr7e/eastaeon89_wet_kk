#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov8830raw.h"
#include "camera_info_ov8830raw.h"
#include "camera_custom_AEPlinetable.h"

const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,

    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	}
    },
    ISPPca: {
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
    },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
    }
    }},
    ISPCcmPoly22:{
        68359, // i4R_AVG
        16625, // i4R_STD
        91292, // i4B_AVG
        22987, // i4B_STD
        { // i4P00[9]
            4550000, -1970000, -17500, -520000, 3277500, -202500, 17500, -2487500, 5030000
        },
        { // i4P10[9]
            772669, -786018, 9969, 62073, -38999, -35171, -48697, 645112, -596415
        },
        { // i4P01[9]
            440797, -715115, 274972, -3731, -444106, 436872, 17336, -579053, 561716
        },
        { // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        { // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        { // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }        
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1136, // u4MinGain, 1024 base = 1x
			10240, // u4MaxGain, 16x
			152, // u4MiniISOGain, ISOxx
			64, // u4GainStepUnit, 1x/8
			27019, // u4PreExpUnit
			31, // u4PreMaxFrameRate
			27019, // u4VideoExpUnit
			31, // u4VideoMaxFrameRate
			1024, // u4Video2PreRatio, 1024 base = 1x
			27859, // u4CapExpUnit
			14, // u4CapMaxFrameRate
			1024, // u4Cap2PreRatio, 1024 base = 1x
			22, // u4LensFno, Fno = 2.8
			350 // u4FocusLength_100x
         },
         // rHistConfig
        {
            2,   // u4HistHighThres
            40,  // u4HistLowThres
            2,   // u4MostBrightRatio
            1,   // u4MostDarkRatio
            160, // u4CentralHighBound
            20,  // u4CentralLowBound
            {240, 230, 220, 210, 200}, // u4OverExpThres[AE_CCT_STRENGTH_NUM]
            {75, 108, 128, 148, 170},  // u4HistStretchThres[AE_CCT_STRENGTH_NUM]
            {18, 22, 26, 30, 34}       // u4BlackLightThres[AE_CCT_STRENGTH_NUM]
        },
        // rCCTConfig
        {
            TRUE,            // bEnableBlackLight
            TRUE,            // bEnableHistStretch
            FALSE,           // bEnableAntiOverExposure
            TRUE,            // bEnableTimeLPF
            FALSE,            // bEnableCaptureThres
            FALSE,            // bEnableVideoThres
            FALSE,            // bEnableStrobeThres
            47,                // u4AETarget
            47,                // u4StrobeAETarget

            50,                // u4InitIndex
            4,                 // u4BackLightWeight
            32,                // u4HistStretchWeight
            4,                 // u4AntiOverExpWeight
            2,                 // u4BlackLightStrengthIndex
            0,                 // u4HistStretchStrengthIndex
            2,                 // u4AntiOverExpStrengthIndex
            2,                 // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8}, // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM]
            90,                // u4InDoorEV = 9.0, 10 base
            -10,               // i4BVOffset delta BV = -2.3
            64,                 // u4PreviewFlareOffset
            64,                 // u4CaptureFlareOffset
            5,                 // u4CaptureFlareThres
            64,                 // u4VideoFlareOffset 
            5,                 // u4VideoFlareThres
            64,                 // u4StrobeFlareOffset
            2,                 // u4StrobeFlareThres
            8,                 // u4PrvMaxFlareThres
            0,                 // u4PrvMinFlareThres
            8,                 // u4VideoMaxFlareThres
            0,                 // u4VideoMinFlareThres            
            18,                // u4FlatnessThres              // 10 base for flatness condition.
            75                 // u4FlatnessStrength
         }
    },

    // AWB NVRAM
    {
    	// AWB calibration data
    	{
    		// rUnitGain (unit gain: 1.0 = 512)
    		{
    			0,	// i4R
    			0,	// i4G
    			0	// i4B
    		},
    		// rGoldenGain (golden sample gain: 1.0 = 512)
    		{
	            0,	// i4R
	            0,	// i4G
	            0	// i4B
            },
    		// rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
    		{
	            0,	// i4R
	            0,	// i4G
	            0	// i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
			   871,	// i4R
	           512,	// i4G
			   610	// i4B
    		}
    	},
    	// Original XY coordinate of AWB light source
    	{
		    // Strobe
		    {
			    132, 	// i4X
			    -261	// i4Y
		    },
    		// Horizon
    		{
			-439,	// i4X
			-268	// i4Y
    		},
    		// A
    		{
			-322,	// i4X
			-283	// i4Y
    		},
    		// TL84
    		{
			-137,	// i4X
			-281	// i4Y
    		},
    		// CWF
    		{
			-95,	// i4X
			-383	// i4Y
    		},
    		// DNP
    		{
			-19,	// i4X
			-296	// i4Y
    		},
    		// D65
    		{
			132,	// i4X
			-261	// i4Y
    		},
		// DF
		{
			0,	// i4X
			0	// i4Y
    		}
    	},
    	// Rotated XY coordinate of AWB light source
    	{
		    // Strobe
		    {
			    123,	// i4X
			    -266	// i4Y
		    },
    		// Horizon
    		{
			-448,	// i4X
			-253	// i4Y
    		},
    		// A
    		{
			-332,	// i4X
			-272	// i4Y
    		},
    		// TL84
    		{
			-147,	// i4X
			-276	// i4Y
    		},
    		// CWF
    		{
			-108,	// i4X
			-380	// i4Y
    		},
    		// DNP
    		{
			-29,	// i4X
			-295	// i4Y
    		},
    		// D65
    		{
			123,	// i4X
			-266	// i4Y
    		},
		// DF
		{
			0,	// i4X
			0	// i4Y
    		}
    	},
	// AWB gain of AWB light source
	{
		// Strobe
		{
			871,	// i4R
			512,	// i4G
			610     // i4B
		},
		// Horizon
		{
			512,	// i4R
			646,	// i4G
			1682     // i4B
		},
		// A
		{
			512,	// i4R
			539,	// i4G
			1224     // i4B
		},
		// TL84
		{
			622,	// i4R
			512,	// i4G
			901     // i4B
		},
		// CWF
		{
			755,	// i4R
			512,	// i4G
			978     // i4B
		},
		// DNP
		{
			745,	// i4R
			512,	// i4G
			784     // i4B
		},
		// D65
		{
			871,	// i4R
			512,	// i4G
			610     // i4B
		},
		// DF
		{
			512,	// i4R
			512,	// i4G
			512     // i4B
		}
	},
    	// Rotation matrix parameter
    	{
		2,	// i4RotationAngle
		256,	// i4Cos
		9	// i4Sin
    	},
    	// Daylight locus parameter
    	{
		-136,	// i4SlopeNumerator
    		128	// i4SlopeDenominator
    	},
    	// AWB light area
    	{
		    // Strobe:FIXME
		    {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
		    },
    		// Tungsten
    		{
            -197,    // i4RightBound
            -847,    // i4LeftBound
            -212,    // i4UpperBound
            -312    // i4LowerBound
    		},
    		// Warm fluorescent
    		{
            -197,    // i4RightBound
            -847,    // i4LeftBound
            -312,    // i4UpperBound
            -432    // i4LowerBound
    		},
    		// Fluorescent
    		{
			-79,	// i4RightBound
			-197,	// i4LeftBound
			-199,	// i4UpperBound
			-328	// i4LowerBound
    		},
    		// CWF
    		{
            -79,    // i4RightBound
            -197,    // i4LeftBound
            -328,    // i4UpperBound
            -430    // i4LowerBound
    		},
    		// Daylight
    		{
            100,    // i4RightBound
            -79,    // i4LeftBound
            -186,    // i4UpperBound
            -346    // i4LowerBound
    		},
    		// Shade
    		{
            508,    // i4RightBound
            100,    // i4LeftBound
            -186,    // i4UpperBound
            -346    // i4LowerBound
		},
		// Daylight Fluorescent
		{
			148,	// i4RightBound
			-79,	// i4LeftBound
			-346,	// i4UpperBound
			-430	// i4LowerBound
    		}
    	},
    	// PWB light area
    	{
    		// Reference area
    		{
			508,
			 -847,
			 -161,
			 -432
    		},

    		// Daylight
    		{
			125,
			 -79,
			 -186,
			 -346
    		},

    		// Cloudy daylight
    		{
			225,
			 50,
			 -186,
			 -346
    		},

    		// Shade
    		{
			325,
			 50,
			 -186,
			 -346
    		},

    		// Twilight
    		{
			-79,
			 -239,
			 -186,
			 -346
    		},

    		// Fluorescent
    		{
			173,
			 -247,
			 -216,
			 -430
    		},

    		// Warm fluorescent
    		{
			-232,
			 -432,
			 -216,
			 -430
    		},

    		// Incandescent
    		{
			-232,
			 -432,
			 -186,
			 -346
    		},
            {// Gray World
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
		    }
    	},
    	// PWB default gain
    	{
    		// Daylight
    		{
			766,
			512,
			701
    		},
    		// Cloudy daylight
    		{
			889,
			 512, 
			 598
    		},
    		// Shade
    		{
			949,
			 512,
			  557
    		},
    		// Twilight
    		{
			604,
			 512, 
			 905
    		},
    		// Fluorescent
    		{
			767,
			 512, 
			 822
    		},
    		// Warm fluorescent
    		{
			517,
			 512, 
			 1242
    		},
    		// Incandescent
    		{
			482,
			 512,
			  1153
		},
		// Gray World
		{
			512,	// i4R
			512,	// i4G
			512	// i4B
    		}
    	},
    	// AWB preference color
    	{
    		// Tungsten
    		{
    			30,	// i4SliderValue
			5588	// i4OffsetThr
    		},
    		// Warm fluorescent
    		{
    			50,	// i4SliderValue
			4362	// i4OffsetThr
    		},
    		// Shade
    		{
    			50,	// i4SliderValue
			841	// i4OffsetThr
    		},
    		// Daylight WB gain
    		{
			715,	// i4R
    			512,	// i4G
			754	// i4B
		},
		// Preference gain: strobe
		{
			512,	// i4R
			512,	// i4G
			512	// i4B
		},
		// Preference gain: tungsten
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
		},
		// Preference gain: warm fluorescent
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
		},
		// Preference gain: fluorescent
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
		},
		// Preference gain: CWF
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
		},
		// Preference gain: daylight
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
		},
		// Preference gain: shade
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
		},
		// Preference gain: daylight fluorescent
		{
			512,	// i4R
			512,	// i4G
			512	    // i4B
    		}
    	},
    	// CCT estimation
    	{
    		// CCT
    		{
			    2300,	// i4CCT[0]
    			2850,	// i4CCT[1]
    			4100,	// i4CCT[2]
    			5100,	// i4CCT[3]
    			6500	// i4CCT[4]
    		},
    		// Rotated X coordinate
    		{
			-571,	// i4RotatedXCoordinate[0]
			-455,	// i4RotatedXCoordinate[1]
			-270,	// i4RotatedXCoordinate[2]
			-152,	// i4RotatedXCoordinate[3]
    			0	// i4RotatedXCoordinate[4]
    		}
    	}
    },
	{0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}};  //  NSFeature



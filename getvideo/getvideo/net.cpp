
#include "DllPlayer.h"
#include "NetSDKDLL.h"
#include <opencv2\opencv.hpp>
#include "windef.h"
#include "colorspace.h"
#include "thread"
#include "mutex"
using namespace cv;
using namespace std;

char* sDVRIP1 = "192.168.1.157";
char* sDVRIP2 = "192.168.1.156";
char* sUserName = "admin";
char* sPassword = "123456";
//void * pUser;
//LONG lPlayerID = 0;

int  CALLBACK  OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, void * pUser,long nReserved2)
 {

	colorspace_init();
	if (pFrameInfo->bIsVideo != 1)
  	{
  		return 0;
  	};
	int m_nColorBit = 32; 
	unsigned char * pRGBbuffer = NULL;
	int MemorySize = pFrameInfo->nWidth * pFrameInfo->nHeight * m_nColorBit / 8;
	pRGBbuffer = (uint8_t *)new char[MemorySize];
	int lpscan = pFrameInfo->nWidth * m_nColorBit/8;
	int convert_ok=YUVToRGB( m_nColorBit, pFrameInfo->nWidth, pFrameInfo->nHeight,(unsigned char*)pBuf,pRGBbuffer,lpscan);					
	if(convert_ok)
	{
		Mat img( pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC4, pRGBbuffer );
//		img.copyTo(image);
//		cout << "bukabukabuka" << n++ << endl;
//		resize( img, img, Size( pFrameInfo->nWidth/2, pFrameInfo->nHeight/2 ) );
		stringstream sout;
		sout << nPort;
		imshow( "nPort"+sout.str(), img );
		waitKey( 20 );
	}
	delete[] pRGBbuffer;
	return 0;

 }

int  CALLBACK  OnMediaDataRecv2(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, void * pUser,long nReserved2)
 {

	colorspace_init();
	if (pFrameInfo->bIsVideo != 1)
  	{
  		return 0;
  	};
	
	int m_nColorBit = 32; 
	unsigned char * pRGBbuffer = NULL;
	int MemorySize = pFrameInfo->nWidth * pFrameInfo->nHeight * m_nColorBit / 8;
	pRGBbuffer = (uint8_t *)new char[MemorySize];
	int lpscan = pFrameInfo->nWidth * m_nColorBit/8;
	int convert_ok=YUVToRGB( m_nColorBit, pFrameInfo->nWidth, pFrameInfo->nHeight,(unsigned char*)pBuf,pRGBbuffer,lpscan);					
	if(convert_ok)
	{
		Mat img( pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC4, pRGBbuffer );
//		img.copyTo(image);
//		cout << "bukabukabuka" << n++ << endl;
//		resize( img, img, Size( pFrameInfo->nWidth/2, pFrameInfo->nHeight/2 ) );
		stringstream sout;
		sout << nPort;
		imshow( "nPort"+sout.str(), img );
		waitKey( 20 );
	}
	delete[] pRGBbuffer;
	return 0;

 }

typedef struct _StateEventParam
{
	LONG lUser;
	LONG nStateCode;
	char pResponse[2048];
	void *pUser;
}StateEventParam;

LONG CALLBACK ffStatusEventCallBack(LONG lUser,LONG nStateCode,char *pResponse,void *pUser)
{

	StateEventParam * pParam=new StateEventParam();
	if (NULL == pParam)
	{
		return -1;
	}
	memset(pParam, 0, sizeof(StateEventParam));

	pParam->lUser=lUser;

	if (nStateCode == EVENT_PTZALARM)
	{
		memcpy(pParam->pResponse, pResponse, sizeof(ALARM_ITEM));
	}
	else if (nStateCode == EVENT_RECVVIDEOPARAM)
	{
		memcpy(pParam->pResponse, pResponse, sizeof(VIDEO_PARAM));	
	}
	else if (nStateCode == EVENT_RECVVIDEOAUDIOPARAM)
	{
		memcpy(pParam->pResponse, pResponse, sizeof(STREAM_AV_PARAM));	
	}
	else if (nStateCode == EVENT_LOGINOK )
	{
		printf( "OK\r\n" );
	}
	else if (nStateCode == EVENT_CONNECTOK)
	{
		printf( "connectok\r\n" ); 
	}
	else
	{
		sprintf_s(pParam->pResponse,2048,"%s",pResponse);
	}

	pParam->nStateCode=nStateCode;
	pParam->pUser=pUser;
	return 0;
}

LONG CALLBACK cbRealDataCallBack(LONG lRealHandle,DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData)
{
	LONG lPlayerID = lRealHandle;
	if( lRealHandle <= 0 )
	{
		return 0;
	}
	if (dwDataType == 0)
	{
			long lInputErr = IP_TPS_InputVideoData( lPlayerID, pBuffer, dwBufSize, pExtData->bIsKey, pExtData->timestamp);
	}
	else if(dwDataType==2)
	{
		
		STREAM_AV_PARAM *pavParam = (STREAM_AV_PARAM *)pBuffer;

		if (pavParam->bHaveVideo)
		{
			int isAudioParam = 0;
			int nBufPoolSize = 1024;	
			long lRet = IP_TPS_OpenStream( lPlayerID, (BYTE *)&(pavParam->videoParam), sizeof(VIDEO_PARAM), isAudioParam, nBufPoolSize);
		}
		if (pavParam->bHaveAudio)
		{
			int isAudioParam = 1;
			int nBufPoolSize = 40;
			long lRet = IP_TPS_OpenStream( lPlayerID, (BYTE *)&(pavParam->audioParam), sizeof(AUDIO_PARAM), isAudioParam, nBufPoolSize);
		}

		IP_TPS_SetDecCallBack( lPlayerID, OnMediaDataRecv, NULL);
		int iRetPlay = IP_TPS_Play( lPlayerID, 0);

	}
	return 0;
}

LONG CALLBACK cbRealDataCallBack2(LONG lRealHandle,DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData)
{
	LONG lPlayerID = lRealHandle;
	if( lRealHandle <= 0 )
	{
		return 0;
	}
	if (dwDataType == 0)
	{
			long lInputErr = IP_TPS_InputVideoData( lPlayerID, pBuffer, dwBufSize, pExtData->bIsKey, pExtData->timestamp);
	}
	else if(dwDataType==2)
	{
		
		STREAM_AV_PARAM *pavParam = (STREAM_AV_PARAM *)pBuffer;

		if (pavParam->bHaveVideo)
		{
			int isAudioParam = 0;
			int nBufPoolSize = 1024;	
			long lRet = IP_TPS_OpenStream( lPlayerID, (BYTE *)&(pavParam->videoParam), sizeof(VIDEO_PARAM), isAudioParam, nBufPoolSize);
		}
		if (pavParam->bHaveAudio)
		{
			int isAudioParam = 1;
			int nBufPoolSize = 40;
			long lRet = IP_TPS_OpenStream( lPlayerID, (BYTE *)&(pavParam->audioParam), sizeof(AUDIO_PARAM), isAudioParam, nBufPoolSize);
		}

		IP_TPS_SetDecCallBack( lPlayerID, OnMediaDataRecv2, NULL);
		int iRetPlay = IP_TPS_Play( lPlayerID, 0);

	}
	return 0;
}


int main()
{
	IP_NET_DVR_Init();//初始化NETSDK
	IP_NET_DVR_SetStatusEventCallBack( ffStatusEventCallBack, "" );//设置状态改变回调函数

	LONG  lUserID1 = IP_NET_DVR_Login( sDVRIP1, 8091, sUserName, sPassword, NULL);//注册设备，成功返回非0的ID
	LONG  lUserID2 = IP_NET_DVR_Login( sDVRIP2, 8091, sUserName, sPassword, NULL);//注册设备，成功返回非0的ID

	USRE_VIDEOINFO user;
	memset(&user, 0x00, sizeof(USRE_VIDEOINFO));
	user.bIsTcp = 0; 
	user.nVideoChannle = 1;
	user.nVideoPort = 554;
	LONG A = IP_NET_DVR_RealPlayEx( lUserID1, sDVRIP1, sUserName, sPassword, cbRealDataCallBack, &user, NULL);//请求视频流
	LONG B = IP_NET_DVR_RealPlayEx( lUserID2, sDVRIP2, sUserName, sPassword, cbRealDataCallBack2, &user, NULL);//请求视频流

	return 0;
}
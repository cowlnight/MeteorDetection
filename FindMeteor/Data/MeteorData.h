<<<<<<< HEAD
/**
* @file MeteorData.h
*
* Classes for application data
*
*/

#ifndef _METEORDATA_H_
#define _METEORDATA_H_



namespace FindMeteor
{
	/**
	*	@brief Class for the application data
	*/
	class CMeteorData
	{
	public:

		CMeteorData(void) { Cleanup(); }
		~CMeteorData(void) { Cleanup(); }

		/// clean up data
		void Cleanup(void)
		{
			m_isTesting = false;
			m_needOutput = false;
			m_doEvaluation = false;
		}

	
	public:
		
		/*  Preparation  */ 
		bool m_isTesting;								// 是否在測試
		bool m_needOutput;							// 是否要輸出計算過程的圖片
		bool m_doEvaluation;							// 是否要對每個步驟與GT進行分析並輸出結果


		// Path
		std::string srcFilePath;								// 圖片路徑
		std::string dataSubPath;								// 圖片路徑
		std::string srcFileName;								// 圖片名稱（不包含型態）
		std::string srcFileNameT;							// 圖片名稱（包含型態）
		std::string filename;								// json檔存放位置（同圖片檔路徑）
		std::string outputFolderPath;

		// Image
		cv::Mat srcLoadImg;									// 儲存輸入的圖片


	};
}

=======
/**
* @file MeteorData.h
*
* Classes for application data
*
*/

#ifndef _METEORDATA_H_
#define _METEORDATA_H_



namespace FindMeteor
{
	/**
	*	@brief Class for the application data
	*/
	class CMeteorData
	{
	public:

		CMeteorData(void) { Cleanup(); }
		~CMeteorData(void) { Cleanup(); }

		/// clean up data
		void Cleanup(void)
		{
			m_isTesting = false;
			m_needOutput = false;
			m_doEvaluation = false;
		}

	
	public:
		
		/*  Preparation  */ 
		bool m_isTesting;								// 是否在測試
		bool m_needOutput;							// 是否要輸出計算過程的圖片
		bool m_doEvaluation;							// 是否要對每個步驟與GT進行分析並輸出結果


		// Path
		std::string srcFilePath;								// 圖片路徑
		std::string dataSubPath;								// 圖片路徑
		std::string srcFileName;								// 圖片名稱（不包含型態）
		std::string srcFileNameT;							// 圖片名稱（包含型態）
		std::string filename;								// json檔存放位置（同圖片檔路徑）
		std::string outputFolderPath;

		// Image
		cv::Mat srcLoadImg;									// 儲存輸入的圖片


	};
}

>>>>>>> CppVersion
#endif
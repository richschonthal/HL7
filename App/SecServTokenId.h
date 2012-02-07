//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SECSERVTOKENID_H_
#define _SECSERVTOKENID_H_

//--------------------------------------------------------------------------------
class SSTokenId
	{
	public:
		enum
			{
			Unknown,

			Viewer2, Viewer3, Viewer4, Viewer5, Viewer6, Viewer7, Viewer8,
			ViewerTest2, ViewerTest3, ViewerTest4, ViewerTest5, ViewerTest6, ViewerTest7, ViewerTest8,

			SSTokenReserved1 = 16,
			SSTokenReserved2 = 32,
			
			HL7_3, HL7_4, HL7_5,
			HL7_3Test, HL7_4Test, HL7_5Test,
			
			TeleRadServer2, TeleRadServer3, TeleRadServer4, TeleRadServer5,
			TeleRadServer2Test, TeleRadServer3Test, TeleRadServer4Test, TeleRadServer5Test,
			
			SSTokenReserved3 = TeleRadServer5Test + 20,
			SSTokenReserved4 = SSTokenReserved3 + 20,
			
			AcqModule1, AcqModule2, AcqModule3, AcqModule4, AcqModule5,
			AcqModule1Test, AcqModule2Test, AcqModule3Test, AcqModule4Test, AcqModule5Test,
			
			MCImageLib1, MCImageLib2, MCImageLib3, MCImageLib4, MCImageLib5,
			MCImageLib1Test, MCImageLib2Test, MCImageLib3Test, MCImageLib4Test, MCImageLib5Test,
			
			SSTokenReserved5 = MCImageLib5Test + 20,
			SSTokenReserved6 = SSTokenReserved5 + 20,
			
			RisHis1, RisHis2, RisHis3, RisHis4, RisHis5,
			RisHis1Test, RisHis2Test, RisHis3Test, RisHis4Test, RisHis5Test,
			
			SSTokenReserved7 = RisHis5Test + 20,
			SSTokenReserved8 = SSTokenReserved7 + 20,
			
			MDServer7, MDServer8, MDServer9, MDServer10,
			MDServer7Test, MDServer8Test, MDServer9Test, MDServer10Test,
			
			ArchiveD1, ArchiveD2, ArchiveD3, ArchiveD4, ArchiveD5,
			ArchiveD1Test, ArchiveD2Test, ArchiveD3Test, ArchiveD4Test, ArchiveD5Test,
			
			SSTokenReserved9 = ArchiveD5Test + 20,
			SSTokenReserved10 = SSTokenReserved9 + 20,
			
			ArchiveAIB1, ArchiveAIB2, ArchiveAIB3, ArchiveAIB4, ArchiveAIB5,
			ArchiveAIB1Test, ArchiveAIB2Test, ArchiveAIB3Test, ArchiveAIB4Test, ArchiveAIB5Test,
			
			CompressServer1, CompressServer2, CompressServer3, CompressServer4, CompressServer5,
			CompressServer1Test, CompressServer2Test, CompressServer3Test, CompressServer4Test, CompressServer5Test,

			SSTokenReserved11 = CompressServer5Test + 20,
			SSTokenReserved12 = SSTokenReserved11 + 20,
			
			WorkListMgr1, WorkListMgr2, WorkListMgr3, WorkListMgr4, WorkListMgr5,
			WorkListMgr1Test, WorkListMgr2Test, WorkListMgr3Test, WorkListMgr4Test, WorkListMgr5Test,

			AdHoc2, AdHoc3, AdHoc4, AdHoc5, AdHoc6, AdHoc7, AdHoc8,
			AdHoc2Test, AdHoc3Test, AdHoc4Test, AdHoc5Test, AdHoc6Test, AdHoc7Test, AdHoc8Test,

			SSTokenReserved13 = AdHoc8Test + 20,
			SSTokenReserved14 = SSTokenReserved13 + 20,
			
			ViewPrint2, ViewPrint3, ViewPrint4, ViewPrint5, ViewPrint6, ViewPrint7, ViewPrint8,
			ViewPrint2Test, ViewPrint3Test, ViewPrint4Test, ViewPrint5Test, ViewPrint6Test, ViewPrint7Test, ViewPrint8Test,

			SSTokenReserved15 = ViewPrint8Test + 20,
			SSTokenReserved16 = SSTokenReserved15 + 20,
			
			DeptServ2, DeptServ3, DeptServ4, DeptServ5, DeptServ6, DeptServ7, DeptServ8,
			DeptServ2Test, DeptServ3Test, DeptServ4Test, DeptServ5Test, DeptServ6Test, DeptServ7Test, DeptServ8Test,

			// used only by the backup security server
			BackupServer,

			SSTokenReserved17 = BackupServer + 20,

			IMS_1, IMS_2, IMS_3, IMS_4, IMS_5, IMS_6,
			IMS_1Test, IMS_2Test, IMS_3Test, IMS_4Test, IMS_5Test, IMS_6Test,

			Digitizer1, Digitizer2, Digitizer3, Digitizer4, Digitizer5, 
			Digitizer1Test, Digitizer2Test, Digitizer3Test, Digitizer4Test, Digitizer5Test, 

			Modality1, Modality2, Modality3, Modality4, Modality5, 
			Modality1Test, Modality2Test, Modality3Test, Modality4Test, Modality5Test,

			HIS_RIS_Bi_1, HIS_RIS_Bi_2, HIS_RIS_Bi_3, HIS_RIS_Bi_4, HIS_RIS_Bi_5, 
			HIS_RIS_Bi_1Test, HIS_RIS_Bi_2Test, HIS_RIS_Bi_3Test, HIS_RIS_Bi_4Test, HIS_RIS_Bi_5Test,

			WorkListClient1, WorkListClient2, WorkListClient3, WorkListClient4, WorkListClient5,
			WorkListClient1Test, WorkListClient2Test, WorkListClient3Test, WorkListClient4Test, WorkListClient5Test,

			ViewQA1, ViewQA2, ViewQA3, ViewQA4, ViewQA5,
			ViewQA1Test, ViewQA2Test, ViewQA3Test, ViewQA4Test, ViewQA5Test,
			};
	};

#endif _SECSERVTOKENID_H_

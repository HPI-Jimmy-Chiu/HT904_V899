namespace HT9045.Core
{
    // Mirrors a representative slice of the BCB6 C++ PROD_INFO_ST (cprod.h,
    // 'extern PROD_INFO_ST Prod;'). The real struct is ~770 lines of motion /
    // offset arrays; the skeleton keeps a faithful cross-section so the team
    // can reverse-look-up field names. Field names match the C++ members.
    public class ProdInfo
    {
        // char cFileName[256];           -- active recipe / setup file name.
        public string CFileName;

        // int  iSiteMap[2][2][8];        -- site enable / mapping table.
        // Flattened representative view: which logical sites are enabled.
        public bool[] SiteOn;

        // Representative front/rear test-Z teach positions (TestZ1_Test etc.).
        public int TestZ1_Test;
        public int TestZ2_Test;

        // int iHotTime;                  -- HotPlate pre-heat dwell.
        public int IHotTime;

        // SM_SHUTTLE InSHT[2] / OutSHT[2] reduce to a count for the skeleton.
        public int ShuttleCount;

        public ProdInfo()
        {
            CFileName = "";
            // 906 default fixture is up to 32 sites (see TesterTCP site grid).
            SiteOn = new bool[32];
            ShuttleCount = 2;
        }
    }
}

string root=@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029";
if(!Mastercam.IO.FileManager.CurrentFileName.StartsWith(root+"\\"))throw new System.Exception("Unexpected document");
foreach(string n in new[]{"baseline","od","length","bore","grip"}){
 if(!Mastercam.IO.FileManager.Open(false,root+"\\"+n+".mcam"))throw new System.Exception("Open");
 int[] ids=Mastercam.Support.SearchManager.GetOperationIDs(true);
 int rc=P.ReadOps(ids,ids.Length,root+"\\"+n+"-ops.json");if(rc!=0)throw new System.Exception("ReadOps");
}
public static class P {
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops\OpsProbe.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl,CharSet=System.Runtime.InteropServices.CharSet.Unicode)] public static extern int ReadOps(int[] ids,int count,string path);
}

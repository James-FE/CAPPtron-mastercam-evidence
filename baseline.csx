string root=@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029";
if(Mastercam.IO.FileManager.CurrentFileName!="T" || Mastercam.Support.SearchManager.GetOperationIDs(true).Length!=0)throw new System.Exception("Expected empty host");
if(!Mastercam.IO.FileManager.Open(false,root+@"\baseline.mcam"))throw new System.Exception("Open failed");
System.IO.File.WriteAllText(root+@"\baseline-identity.json","{\"file\":\""+Mastercam.IO.FileManager.CurrentFileName.Replace("\\","\\\\")+"\",\"metric\":"+Mastercam.IO.SettingsManager.Metric.ToString().ToLowerInvariant()+",\"ids\":["+string.Join(",",Mastercam.Support.SearchManager.GetOperationIDs(true))+"]}");
int rc=P.ReadStock(root+@"\baseline-stock.json");
System.IO.File.WriteAllText(root+@"\baseline-return.txt",rc.ToString());
public static class P {
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build\ParameterProbe.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl,CharSet=System.Runtime.InteropServices.CharSet.Unicode)] public static extern int ReadStock(string path);
}

string root=@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029";
if(!Mastercam.IO.FileManager.CurrentFileName.StartsWith(root+"\\"))throw new System.Exception("Unexpected document");
string[] names={"offset","origin-world-x","finish-feed","finish-rpm","finish-stock-x","finish-stock-z"}; double[] values={2,1,-0.25,290,0.03,0.04};
for(int i=0;i<names.Length;i++){
 string n=names[i];string path=root+"\\"+n+".mcam";
 if(!Mastercam.IO.FileManager.Open(false,path))throw new System.Exception("Open failed "+n);
 if(!Mastercam.IO.SettingsManager.Metric)throw new System.Exception("Units");
 if(P.ReadOps(Mastercam.Support.SearchManager.GetOperationIDs(true),Mastercam.Support.SearchManager.GetOperationIDs(true).Length,root+"\\"+n+"-before.json")!=0)throw new System.Exception("Read before");
 int rc=P.ChangeOp(i+1,values[i]);System.IO.File.WriteAllText(root+"\\"+n+"-write-return.txt",rc.ToString());if(rc!=0)throw new System.Exception("Write failed");
 if(P.ReadOps(Mastercam.Support.SearchManager.GetOperationIDs(true),Mastercam.Support.SearchManager.GetOperationIDs(true).Length,root+"\\"+n+"-after.json")!=0)throw new System.Exception("Read after");
 if(!Mastercam.IO.FileManager.SaveAs(path))throw new System.Exception("Save failed");
 System.IO.File.WriteAllText(root+"\\"+n+"-saved.txt","true");
 if(!Mastercam.IO.FileManager.New(false,false))throw new System.Exception("Close failed");
 if(!Mastercam.IO.FileManager.Open(false,path))throw new System.Exception("Reopen failed");
 if(P.ReadOps(Mastercam.Support.SearchManager.GetOperationIDs(true),Mastercam.Support.SearchManager.GetOperationIDs(true).Length,root+"\\"+n+"-reopened.json")!=0)throw new System.Exception("Read reopened");
 System.IO.File.WriteAllText(root+"\\"+n+"-ids.txt",string.Join(",",Mastercam.Support.SearchManager.GetOperationIDs(true)));
}
public static class P {
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-v2\OpsProbeV2.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl,CharSet=System.Runtime.InteropServices.CharSet.Unicode)] public static extern int ReadOps(int[] ids,int count,string path);
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-v2\OpsProbeV2.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl)] public static extern int ChangeOp(int field,double value);
}


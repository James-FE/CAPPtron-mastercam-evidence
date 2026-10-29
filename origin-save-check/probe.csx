string root=@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029";string test=root+@"\origin-save-check";
if(!Mastercam.IO.FileManager.CurrentFileName.StartsWith(root+"\\"))throw new System.Exception("Unexpected document");
foreach(string mode in new[]{"save","saveas"}){
 string input=test+"\\"+mode+"-input.mcam";string output=mode=="save"?input:test+@"\saveas-new-output.mcam";
 if(mode=="saveas" && System.IO.File.Exists(output))throw new System.Exception("Output exists");
 if(!Mastercam.IO.FileManager.Open(false,input))throw new System.Exception("Open");
 var ids=Mastercam.Support.SearchManager.GetOperationIDs(true);
 P.ReadOps(ids,ids.Length,test+"\\"+mode+"-before.json");
 if(P.ChangeOp(2,2)!=0||P.ChangeOp(3,-0.23)!=0)throw new System.Exception("Write");
 P.ReadOps(ids,ids.Length,test+"\\"+mode+"-modified.json");
 bool saved=mode=="save"?Mastercam.IO.FileManager.Save():Mastercam.IO.FileManager.SaveAs(output);
 System.IO.File.WriteAllText(test+"\\"+mode+"-save.txt","saved="+saved+"\ncurrent="+Mastercam.IO.FileManager.CurrentFileName+"\nexists="+System.IO.File.Exists(output)+"\nsize="+new System.IO.FileInfo(output).Length);
 if(!saved)throw new System.Exception("Save failed");
 P.ReadOps(ids,ids.Length,test+"\\"+mode+"-after-save.json");
 if(!Mastercam.IO.FileManager.New(false,false)||!Mastercam.IO.FileManager.Open(false,output))throw new System.Exception("Reopen");
 P.ReadOps(Mastercam.Support.SearchManager.GetOperationIDs(true),ids.Length,test+"\\"+mode+"-reopened.json");
}
public static class P {
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-v3\OpsProbeV3.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl,CharSet=System.Runtime.InteropServices.CharSet.Unicode)] public static extern int ReadOps(int[] ids,int count,string path);
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-v3\OpsProbeV3.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl)] public static extern int ChangeOp(int field,double value);
}

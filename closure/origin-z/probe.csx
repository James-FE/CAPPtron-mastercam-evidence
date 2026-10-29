string root=@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029";string d=root+@"\closure\origin-z";
void Log(string s){System.IO.File.AppendAllText(d+@"\test.log",s+"\n");}
void Snapshot(string tag){var ids=Mastercam.Support.SearchManager.GetOperationIDs(true);if(P.ReadOps(ids,ids.Length,d+"\\"+tag+".json")!=0)throw new System.Exception("snapshot");P.ReadGroupPlane(d+"\\"+tag+"-group.json");foreach(var v in Mastercam.IO.ViewManager.GetAllViews(false))if(v.ViewID==1||v.ViewName=="CAPABILITY_ORIGIN_Z2")Log(tag+" plane="+v.ViewID+" name="+v.ViewName+" locked="+v.IsLockedView+" origin="+v.ViewOrigin+" matrix="+v.ViewMatrix);}
if(!Mastercam.IO.FileManager.CurrentFileName.StartsWith(root+"\\"))throw new System.Exception("Unexpected document");
if(!Mastercam.IO.FileManager.Open(false,d+@"\input.mcam"))throw new System.Exception("open");
Snapshot("before");
Mastercam.Database.MCView source=null;foreach(var v in Mastercam.IO.ViewManager.GetAllViews(false))if(v.ViewID==1)source=v;
if(source==null)throw new System.Exception("missing plane1");
var plane=new Mastercam.Database.MCView();plane.ViewName="CAPABILITY_ORIGIN_Z2";plane.ViewMatrix=source.ViewMatrix;plane.ViewOrigin=new Mastercam.Math.Point3D(0,0,2);plane.WorkOffsetNumber=source.WorkOffsetNumber;
bool committed=plane.Commit();Log("plane.Commit="+committed+" id="+plane.ViewID);if(!committed)throw new System.Exception("plane commit");
int bound=P.GroupPlane(plane.ViewID,2,Mastercam.Support.SearchManager.GetOperationIDs(true),7,2);Log("BindPlane="+bound);if(bound!=0)throw new System.Exception("bind");
Snapshot("modified");string output=d+@"\persisted-output.mcam";if(System.IO.File.Exists(output))throw new System.Exception("exists");
bool saved=Mastercam.IO.FileManager.SaveAs(output);Log("SaveAs="+saved);if(!saved)throw new System.Exception("save");
if(!Mastercam.IO.FileManager.New(false,false)||!Mastercam.IO.FileManager.Open(false,output))throw new System.Exception("reopen");Snapshot("reopened");Log("current="+Mastercam.IO.FileManager.CurrentFileName);
public static class P {
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-axis\OpsAxisProbe.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl,CharSet=System.Runtime.InteropServices.CharSet.Unicode)]public static extern int ReadGroupPlane(string path);
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-axis\OpsAxisProbe.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl,CharSet=System.Runtime.InteropServices.CharSet.Unicode)]public static extern int ReadOps(int[] ids,int count,string path);
[System.Runtime.InteropServices.DllImport(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\build-ops-axis\OpsAxisProbe.dll",CallingConvention=System.Runtime.InteropServices.CallingConvention.Cdecl)]public static extern int GroupPlane(int id,double x,int[] ids,int count,int axis);
}



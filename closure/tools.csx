#r "D:\Program Files\Mastercam 2026\ToolNetApi.dll"
var m=Cnc.Tool.Interop.TlServices.GetTlMgr();var s=new System.Text.StringBuilder();
s.AppendLine("manager="+m.GetType().FullName);foreach(var t in m.GetLatheTools()){s.AppendLine("tool="+t.GetType().FullName);foreach(var p in t.GetType().GetProperties())if(p.Name.Contains("Id")||p.Name.Contains("ID")||p.Name.Contains("Name")||p.Name.Contains("Slot")||p.Name.Contains("Radius")){try{s.AppendLine(p.Name+"="+p.GetValue(t,null));}catch{}}}
foreach(var mm in m.GetType().GetMethods())if(mm.Name.Contains("Save")||mm.Name.Contains("Update")||mm.Name.Contains("Write")||mm.Name.Contains("Commit")||mm.Name.Contains("Operation"))s.AppendLine(mm.ToString());
System.IO.File.WriteAllText(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\closure\tools.txt",s.ToString());

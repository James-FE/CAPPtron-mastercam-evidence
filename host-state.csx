string J(string s) { return s == null ? "null" : "\"" + s.Replace("\\", "\\\\").Replace("\"", "\\\"").Replace("\r", "\\r").Replace("\n", "\\n").Replace("\t", "\\t") + "\""; }
var ops = Mastercam.Support.SearchManager.GetOperations();
var groups = Mastercam.Support.SearchManager.GetGroups();
var result = "{\"file\":" + J(Mastercam.IO.FileManager.CurrentFileName) + ",\"metric\":" + Mastercam.IO.SettingsManager.Metric.ToString().ToLowerInvariant() + ",\"operationCount\":" + ops.Length + ",\"groupCount\":" + groups.Length + "}";
System.IO.File.WriteAllText(@"D:\GPT_workspace\CNC_project\integration-20261027\evidence\mastercam-capability-20261029\host-before.json",result);


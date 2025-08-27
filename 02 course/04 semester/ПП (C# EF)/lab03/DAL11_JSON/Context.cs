using System.Text.Json;

namespace DAL11_JSON
{
    public class JSONContext : IDisposable
    {
        FileStream fs;
        public List<WSRef> WSRefs { get; private set; }
        public List<Comment> Comments
        {
            get
            {
                List<Comment> rc = new List<Comment>();
                WSRefs.ForEach(wsref => { wsref.Comments?.ForEach(comment => rc.Add(comment)); });
                return rc;
            }
        }
        private JSONContext(string FileName)
        {
            if (!File.Exists(FileName))
            {
                fs = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.ReadWrite);
                JsonSerializer.SerializeAsync<List<WSRef>>(fs, WSRefs = new List<WSRef>()).Wait();
            }
            else
            {
                fs = new FileStream(FileName, FileMode.Open, FileAccess.ReadWrite);
                WSRefs = Load();
            }
        }
        public static JSONContext Create(string FileName) { return new JSONContext(FileName); }
        private List<WSRef> Load()
        {
            fs.Seek(0, SeekOrigin.Begin);
            List<WSRef>? wsrefs = JsonSerializer.DeserializeAsync<List<WSRef>?>(fs).Result;
            return (wsrefs == null) ? new List<WSRef>() : wsrefs;
        }
        public int SaveChanges()
        {
            fs.Seek(0, SeekOrigin.Begin);
            JsonSerializer.SerializeAsync<List<WSRef>>(fs, WSRefs == null ? new List<WSRef>() : WSRefs);
            return 1;
        }
        private int MaxWSRefsId()
        {
            int rc = 0;
            if (WSRefs.Count > 0) rc = WSRefs.Max(wsref => wsref.Id);
            return rc;
        }
        public bool addWSRef(WSRef wsref)
        {
            wsref.Id = MaxWSRefsId() + 1;
            WSRefs.Add(wsref);
            return true;
        }
        private int MaxCommentsId()
        {
            int rc = 0;
            WSRefs.ForEach(wsref => {
                if (wsref.Comments != null && wsref.Comments.Any())
                {
                    int? m = wsref.Comments.Max(comment => comment.Id);
                    if (m.HasValue && m > rc) rc = (int)m;
                }
            });
            return rc;
        }
        public bool addComment(Comment comment)
        {
            bool rc = false;
            int idx = WSRefs.FindIndex(wsref => wsref.Id == comment.WSrefId);
            if (rc = (idx >= 0))
            {
                if (WSRefs[idx].Comments == null) WSRefs[idx].Comments = new List<Comment>();
                comment.Id = MaxCommentsId() + 1;
                WSRefs[idx].Comments.Add(comment);
            }
            return rc;
        }
        public void Dispose() { fs.Dispose(); }
    }
}

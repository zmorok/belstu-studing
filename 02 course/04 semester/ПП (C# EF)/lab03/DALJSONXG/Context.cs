using System.Text.Json;

namespace DALJSONXG
{
    public class Context : IDisposable
    {
        private readonly string fileName = "WSRef_XG.json";
        private List<WSRef> wsRefs;
        public Context()
        {
            if (!File.Exists(fileName)) { wsRefs = new List<WSRef>(); SaveChanges(); }
            else { wsRefs = Load(); }
        }
        private List<WSRef> Load()
        {
            using var fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            return JsonSerializer.Deserialize<List<WSRef>>(fs) ?? new List<WSRef>();
        }
        public int SaveChanges()
        {
            using var fs = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            JsonSerializer.Serialize(fs, wsRefs);
            return 1;
        }
        public List<WSRef> WSRefs => wsRefs;
        public List<Comment> Comments
        {
            get
            {
                return wsRefs.SelectMany(wsref => wsref.Comments).ToList();
            }
        }
        private int MaxWSRefsId() { return wsRefs.Any() ? wsRefs.Max(wsref => wsref.Id) : 0; }
        public bool AddWSRef(WSRef wsref)
        {
            wsref.Id = MaxWSRefsId() + 1;
            wsRefs.Add(wsref);
            return true;
        }
        private int MaxCommentsId() { return Comments.Any() ? Comments.Max(comment => comment.Id) : 0; }
        public bool AddComment(Comment comment)
        {
            var wsRef = wsRefs.FirstOrDefault(w => w.Id == comment.WSrefId);
            if (wsRef == null) return false;
            comment.Id = MaxCommentsId() + 1;
            wsRef.Comments.Add(comment);
            return true;
        }
        public void Dispose() { }
    }
}

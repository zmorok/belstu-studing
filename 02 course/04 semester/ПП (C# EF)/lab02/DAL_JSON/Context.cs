using System;
using System.IO;
using System.Text.Json;
using Repo;

namespace DAL_JSON
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
                this.WSRefs.ForEach(wsref =>
                {
                    wsref.Comments?.ForEach(comment => rc.Add(comment));
                });
                return rc;
            }
        }

        private JSONContext(string FileName)
        {
            if (!File.Exists(FileName))
            {
                this.fs = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.ReadWrite);
                JsonSerializer
                    .SerializeAsync<List<WSRef>>(this.fs, this.WSRefs = new List<WSRef>())
                    .Wait();
            }
            else
            {
                this.fs = new FileStream(FileName, FileMode.Open, FileAccess.ReadWrite);
                this.WSRefs = this.Load();
            }
        }

        public static JSONContext Create(string FileName)
        {
            return new JSONContext(FileName);
        }

        private List<WSRef> Load()
        {
            this.fs.Seek(0, SeekOrigin.Begin);
            List<WSRef>? wsrefs = JsonSerializer.DeserializeAsync<List<WSRef>?>(fs).Result;
            return (wsrefs == null) ? new List<WSRef>() : wsrefs;
        }

        public int SaveChanges()
        {
            this.fs.Seek(0, SeekOrigin.Begin);
            JsonSerializer.SerializeAsync<List<WSRef>>(
                this.fs,
                this.WSRefs == null ? new List<WSRef>() : this.WSRefs
            );
            return 1;
        }

        private int? MaxWSRefsId()
        {
            int? rc = 0;
            if (this.WSRefs.Count > 0)
                rc = this.WSRefs.Max(wsref => wsref.Id);
            return rc;
        }

        public bool addWSRef(WSRef wsref)
        {
            wsref.Id = MaxWSRefsId() + 1;
            this.WSRefs.Add(wsref);
            return true;
        }

        private int MaxCommentsId()
        {
            int rc = 0;
            int? m = 0;
            this.WSRefs.ForEach(wsref =>
            {
                m = wsref.Comments.Max(comment => comment.Id);
                rc = (m != null && m > rc) ? (int)m : rc;
            });
            return rc;
        }

        public bool addComment(Comment comment)
        {
            bool rc = false;
            int idx = this.WSRefs.FindIndex(wsref => wsref.Id == comment.WSrefId);
            if (rc = (idx >= 0))
            {
                comment.Id = this.MaxCommentsId() + 1;
                this.WSRefs[idx].Comments.Add(comment);
            }
            return rc;
        }

        public void Dispose()
        {
            fs.Dispose();
        }
    }
}

using REPO;

namespace DALMSSQLX
{
    public class Repository : IRepository
    {
        Context context;
        private Repository() { context = new Context(); }
        public static IRepository Create() { return new Repository(); }
        public List<Comment> getAllComment() { return context.Comments.ToList<Comment>(); }
        public List<WSRef> getAllWSRef() { return context.WSRefs.ToList<WSRef>(); }
        public bool addWSRef(WSRef wsref)
        {
            bool rc = false;
            context.Database.BeginTransaction();
            context.WSRefs.Add(wsref);
            rc = (context.SaveChanges() > 0);
            context.Database.CommitTransaction();
            return rc;
        }
        public bool addComment(Comment comment)
        {
            bool rc = false;
            var wsRef = context.WSRefs.Find(comment.WSrefId);
            if (wsRef != null && context.Comments != null)
            {
                using var transaction = context.Database.BeginTransaction();
                context.Comments.Add(comment);
                rc = (context.SaveChanges() > 0);
                transaction.Commit();
            }
            return rc;
        }
        public void Dispose() { context.Dispose(); }
        public Comment? GetCommentById(int Id) { return context.Comments.FirstOrDefault<Comment>((c) => c.Id == Id); }
    }
}

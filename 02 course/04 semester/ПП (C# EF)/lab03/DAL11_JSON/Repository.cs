namespace DAL11_JSON
{
    public class Repository : IRepository
    {
        JSONContext context;
        private Repository() { context = JSONContext.Create("WSRef.json"); }
        public static IRepository Create() { return new Repository(); }
        public List<Comment> getAllComment() { return context.Comments == null ? new List<Comment>() : context.Comments; }
        public List<WSRef> getAllWSRef() { return context.WSRefs == null ? new List<WSRef>() : context.WSRefs; }
        public bool addWSRef(WSRef wsref)
        {
            bool rc = false;
            if (context.addWSRef(wsref)) rc = (context.SaveChanges() > 0);
            return rc;
        }
        public bool addComment(Comment comment)
        {
            bool rc = false;
            if (context.addComment(comment)) rc = (context.SaveChanges() > 0);
            return rc;
        }
        public void Dispose() { context.Dispose(); }
    }
}

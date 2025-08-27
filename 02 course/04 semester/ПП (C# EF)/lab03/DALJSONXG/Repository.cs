namespace DALJSONXG
{
    public interface IRepository : GREPO.IRepository<WSRef, Comment> { }

    public class Repository : IRepository
    {
        private readonly Context context;
        private Repository() { context = new Context(); }
        public static IRepository Create() { return new Repository(); }
        public List<WSRef> getAllWSRef() { return context.WSRefs; }
        public List<Comment> getAllComment() { return context.Comments; }
        public Comment? GetCommentById(int Id) { return context.Comments.FirstOrDefault(c => c.Id == Id); }
        public bool addWSRef(WSRef wsRef)
        {
            bool rc = false;
            if (context.AddWSRef(wsRef))
            {
                context.SaveChanges();
                rc = true;
            }
            return rc;
        }
        public bool addComment(Comment comment)
        {
            bool rc = false;
            if (context.AddComment(comment))
            {
                context.SaveChanges();
                rc = true;
            }
            return rc;
        }
        public void Dispose() { context.Dispose(); }
    }
}

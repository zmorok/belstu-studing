namespace DALMSSQLXG
{
    public interface IRepository : GREPO.IRepository<WSRef, Comment> { }

    public class Repository : IRepository
    {
        private readonly Context context;
        public static IRepository Create() { return new Repository(new Context()); }
        private Repository(Context context) { this.context = context ?? throw new ArgumentNullException(nameof(context)); }
        public List<WSRef> getAllWSRef() { return context.WSRefs.ToList(); }
        public List<Comment> getAllComment() { return context.Comments.ToList(); }
        public Comment? GetCommentById(int Id) { return context.Comments.FirstOrDefault(c => c.Id == Id); }
        public bool addWSRef(WSRef wsRef)
        {
            if (wsRef == null) return false;
            try
            {
                context.WSRefs.Add(wsRef);
                context.SaveChanges();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        public bool addComment(Comment comment)
        {
            bool rc = false;
            if (comment != null)
            {
                try
                {
                    context.Comments.Add(comment);
                    context.SaveChanges();
                    rc = true;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Ошибка при добавлении: {ex.Message}");
                }
            }
            return rc;
        }
        public void Dispose() { context.Dispose(); }
    }
}

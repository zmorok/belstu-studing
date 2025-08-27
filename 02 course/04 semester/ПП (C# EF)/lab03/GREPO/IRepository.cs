namespace GREPO
{
    public interface IRepository<T1, T2> : IDisposable
    {
        List<T1> getAllWSRef();
        List<T2> getAllComment();
        T2? GetCommentById(int Id);
        bool addWSRef(T1 wsRef);
        bool addComment(T2 comment);
    }
}

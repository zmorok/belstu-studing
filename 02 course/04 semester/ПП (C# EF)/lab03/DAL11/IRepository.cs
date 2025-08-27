namespace DAL11
{
    public interface IRepository : IDisposable
    {
        List<WSRef> getAllWSRef();          // получить весь перечень Интернет-ресурсов
        List<Comment> getAllComment();      // получить весь комментарии
        bool addWSRef(WSRef wsref);         // добавить новый Интернет-ресурс
        bool addComment(Comment comment);   // добавить комментарий
    }
}

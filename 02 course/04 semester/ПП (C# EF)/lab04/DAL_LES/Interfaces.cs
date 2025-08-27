namespace DAL_LES
{
    public interface ICelebrity<T> : IDisposable
    {
        List<T> GetAllCelebrities();
        T? GetCelebrityById(int id);
        bool DelCelebrity(int id);
        bool AddCelebrity(T celebrity);
        bool UpdateCelebrity(int id, T celebrity);
    }

    public interface ILifeevent<T> : IDisposable
    {
        List<T> GetAllLifeevents();
        T? GetLifeeventById(int id);
        bool DelLifeevent(int id);
        bool AddLifeevent(T lifeevent);
        bool UpdateLifeevent(int id, T lifeevent);
    }

    public interface IRepository : ICommon, ICelebrity, ILifeevent { }

    public interface ICelebrity : ICelebrity<Celebrity> { }
    public interface ILifeevent : ILifeevent<Lifeevent> { }
    public interface ICommon : ICommon<Celebrity, Lifeevent> { }

    public interface ICommon<T1, T2>
    {
        List<T2> GetLifeeventsByCelebrityId(int celebrityId);
        T1? GetCelebrityByLifeeventId(int lifeeventId);
    }
}
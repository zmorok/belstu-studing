using System.Numerics;

namespace DAL_Celebrity
{
    public interface IRepository<T1, T2> :  IMix<T1, T2>, ICelebrity<T1>, ILifeevent<T2> { }
   
    public  interface IMix<T1, T2>      
    {
        List<T2> GetLifeeventsByCelebrityId(int celebrityId);   // получить все События по Id Знаменитости
        T1? GetCelebrityByLifeeventId(int lifeeventId);         // получить Знаменитость по Id События
    }
    public interface ICelebrity<T> : IDisposable
    {
        List<T> GetAllCelebrities();                      // получить все Знаменитости 
        T? GetCelebrityById(int Id);                      // получить Знаменитость по Id 
        bool DelCelebrity(int id);                        // удалить Знаменитость по Id 
        bool AddCelebrity(T celebrity);                   // добавить Знаменитость  
        bool UpdCelebrity(int id, T celebrity);           // изменить Знаменитость по  Id
        int GetCelebrityIdByName(string name);            // получить первый Id по вхождению подстроки                                                      // 
    }
    public interface ILifeevent<T> : IDisposable
    {
        List<T> GetAllLifeevents();                       // получить все События 
        T? GetLifeevetById(int Id);                       // получить Событие по Id 
        bool DelLifeevent(int id);                        // удалить Событие  по Id 
        bool AddLifeevent(T lifeevent);                   // добавить Событие  
        bool UpdLifeevent(int id, T lifeevent);           // изменить обытие по  Id  
    }
}
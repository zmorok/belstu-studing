using DAL_Celebrity;

namespace DAL_Celebrity_MSSQL;

public interface IRepository : IRepository<Celebrity, Lifeevent> { }

public class Celebrity  //  Знаменитость  
{
    public Celebrity() { this.FullName = string.Empty; this.Nationality = string.Empty; }
    public int Id { get; set; }                        // Id Знаменитости        
    public string FullName { get; set; }         // полное имя   Знаменитости
    public string Nationality { get; set; }         // гражданство  Знаменитости ( 2 символа ISO )
    public string? ReqPhotoPath { get; set; }         // reguest path  Фотографии   
    public virtual bool Update(Celebrity celebrity)   // --вспомогательный метод  
    {
        if (!string.IsNullOrEmpty(celebrity.FullName)) this.FullName = celebrity.FullName;
        if (!string.IsNullOrEmpty(celebrity.Nationality)) this.Nationality = celebrity.Nationality;
        if (!string.IsNullOrEmpty(celebrity.ReqPhotoPath)) this.ReqPhotoPath = celebrity.ReqPhotoPath;
        return true;     //  изменения были ?
    }
}

public class Lifeevent  //  Событие в  жизни знаменитости 
{
    public Lifeevent() { this.Description = string.Empty; }
    public int Id { get; set; }           // Id События  
    public int CelebrityId { get; set; }           // Id Знаменитости
    public DateTime? Date { get; set; }           // дата События 
    public string Description { get; set; }           // описание События 
    public string? ReqPhotoPath { get; set; }           // reguest path  Фотографии
    public virtual bool Update(Lifeevent lifeevent)       // -- вспомогательный метод                                           
    {
        if (!(lifeevent.CelebrityId <= 0)) this.CelebrityId = lifeevent.CelebrityId;
        if (!lifeevent.Date.Equals(new DateTime())) this.Date = lifeevent.Date;
        if (!string.IsNullOrEmpty(lifeevent.Description)) this.Description = lifeevent.Description;
        if (!string.IsNullOrEmpty(lifeevent.ReqPhotoPath)) this.ReqPhotoPath = lifeevent.ReqPhotoPath;
        return true;     //  изменения были ?
    }
}
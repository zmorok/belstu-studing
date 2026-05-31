using Microsoft.EntityFrameworkCore;

namespace DAL_Celebrity_MSSQL;

public class Repository : IRepository
{
    Context context;
    public Repository() { context = new Context(); }
    public Repository(string connectionString) { context = new Context(connectionString); }
    public static IRepository Create() { return new Repository(); }
    public static IRepository Create(string connectionString) { return new Repository(connectionString); }
    public List<Celebrity> GetAllCelebrities() { return context.Celebrities.ToList<Celebrity>(); }
    public Celebrity? GetCelebrityById(int Id) => context.Celebrities.FirstOrDefault(c => c.Id == Id);
    public bool AddCelebrity(Celebrity celebrity)
    {
        context.Celebrities.Add(celebrity);
        return context.SaveChanges() > 0;
    }
    public bool DelCelebrity(int id)
    {
        var celebrity = context.Celebrities.Find(id);
        if (celebrity == null) return false;

        context.Celebrities.Remove(celebrity);
        return context.SaveChanges() > 0;
    }
    public bool UpdCelebrity(int id, Celebrity celebrity)
    {
        var existing = context.Celebrities.Find(id);
        if (existing == null) return false;

        existing.FullName = celebrity.FullName;
        existing.Nationality = celebrity.Nationality;
        existing.ReqPhotoPath = celebrity.ReqPhotoPath;

        return context.SaveChanges() > 0;
    }
    public List<Lifeevent> GetAllLifeevents() { return context.Lifeevents.ToList<Lifeevent>(); }
    public Lifeevent? GetLifeevetById(int Id) => context.Lifeevents.FirstOrDefault(l => l.Id == Id);
    public bool AddLifeevent(Lifeevent lifeevent)
    {
        context.Lifeevents.Add(lifeevent);
        return context.SaveChanges() > 0;
    }
    public bool DelLifeevent(int id)
    {
        var lifeevent = context.Lifeevents.Find(id);
        if (lifeevent == null) return false;

        context.Lifeevents.Remove(lifeevent);
        return context.SaveChanges() > 0;
    }
    public bool UpdLifeevent(int id, Lifeevent lifeevent)
    {
        var existing = context.Lifeevents.Find(id);
        if (existing == null) return false;

        existing.Date = lifeevent.Date;
        existing.Description = lifeevent.Description;
        existing.ReqPhotoPath = lifeevent.ReqPhotoPath;
        existing.CelebrityId = lifeevent.CelebrityId;

        return context.SaveChanges() > 0;
    }
    public List<Lifeevent> GetLifeeventsByCelebrityId(int celebrityId) 
    { 
        return context.Lifeevents.Where(l => l.CelebrityId == celebrityId).ToList(); 
    }
    public Celebrity? GetCelebrityByLifeeventId(int lifeeventId)
    {
        var lifeevent = context.Lifeevents.AsNoTracking().FirstOrDefault(l => l.Id == lifeeventId);
        if (lifeevent == null) return null;
        return context.Celebrities.AsNoTracking().FirstOrDefault(c => c.Id == lifeevent.CelebrityId);
    }
    public int GetCelebrityIdByName(string name)
    {
        Celebrity? celeb = context.Celebrities.FirstOrDefault(c => EF.Functions.Like(c.FullName, $"%{name}%"));
        return celeb?.Id ?? -1;
    }
    public void Dispose()
    {
        context.Dispose();
    }
}
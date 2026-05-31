namespace DAL004
{
    public interface IRepository : IDisposable
    {
        string BasePath { get; }
        Celebrity[] getAllCelebrities();
        Celebrity? getCelebrityById(int id);
        Celebrity[] getCelebritiesBySurname(string surname);
        string? getPhotoPathById(int id);

        int? addCelebrity(Celebrity celebrity);
        bool delCelebrityById(int id);
        int? updCelebrityById(int id, Celebrity celebrity);
        int SaveChanges();
    }

    public record Celebrity (int Id, string Firstname, string Surname, string PhotoPath);
}

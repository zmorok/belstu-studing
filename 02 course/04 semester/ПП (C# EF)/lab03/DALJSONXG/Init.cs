using GREPO;

namespace DALJSONXG
{
    public class Init
    {
        public static void Execute()
        {
            using (IRepository<WSRef, Comment> repo = Repository.Create())
            {
                if (repo.getAllComment().Count == 0)
                {
                    repo.addWSRef(new WSRef() { Description = "Oracle, DMBS, PL/SQL", Url = @"https://www.oracle.com", Minus = 1, Plus = 3 });
                    repo.addComment(new Comment() { WSrefId = 1, Commtext = "very useful link", Stamp = DateTime.Now });
                    repo.addWSRef(new WSRef() { Description = "Java, Jakarta, Java SE, J2EE", Url = @"https://jakarta.ee/", Minus = 2, Plus = 5 });
                    repo.addComment(new Comment() { WSrefId = 2, Commtext = "deprecated information", Stamp = DateTime.Now });
                }
            }
        }
    }
}

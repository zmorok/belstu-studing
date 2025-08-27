namespace DALMSSQLXG
{
    public class Init
    {
        public static void Execute()
        {
            using var context = new Context();
            if (!context.WSRefs.Any())
            {
                var refs = new List<WSRef>()
                {
                    new WSRef() { Description = "Oracle", Url = "https://www.oracle.com", Minus = 0, Plus = 0 },
                    new WSRef() { Description = "Java", Url = "https://jakarta.ee/", Minus = 0, Plus = 0 },
                    new WSRef() { Description = "JavaScript", Url = "https://ecma-international.org/", Minus = 0, Plus = 0 }
                };
                context.WSRefs.AddRange(refs);
                context.SaveChanges();

                var comments = new List<Comment>();
                foreach (var wsRef in refs)
                {
                    comments.Add(new Comment() { WSrefId = wsRef.Id, Stamp = DateTime.Now, Commtext = $"{wsRef.Id}-Comment1" });
                    comments.Add(new Comment() { WSrefId = wsRef.Id, Stamp = DateTime.Now, Commtext = $"{wsRef.Id}-Comment2" });
                }
                context.Comments.AddRange(comments);
                context.SaveChanges();
            }
        }
    }
}

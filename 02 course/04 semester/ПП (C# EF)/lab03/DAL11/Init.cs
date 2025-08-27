namespace DAL11
{
    public class Init
    {
        public static void Execute()
        {
            Context context = new Context();

            if (context.WSRefs != null && context.Comments != null)
            {
                context.Database.EnsureCreated();
                if (!context.WSRefs.Any())
                {
                    List<WSRef> refs = new List<WSRef>()
                    {
                        new WSRef(){ Description="Oracle",     Url = @"https://www.oracle.com", Minus = 0, Plus = 0 },
                        new WSRef(){ Description="Java",       Url = @"https://jakarta.ee/", Minus = 0, Plus = 0 },
                        new WSRef(){ Description="JavaScript", Url = @"https://ecma-international.org/publications-and-standards/standards/ecma-262/", Minus = 0, Plus = 0 }

                    };
                    context.WSRefs.AddRange(refs);
                    context.SaveChanges();
                    List<Comment> comments = new List<Comment>();
                    foreach (WSRef wsRef in refs)
                    {

                        comments.Add(new Comment() { WSrefId = wsRef.Id, Stamp = DateTime.Now, Commtext = wsRef.Id.ToString() + "-Comment1" });
                        comments.Add(new Comment() { WSrefId = wsRef.Id, Stamp = DateTime.Now, Commtext = wsRef.Id.ToString() + "-Comment2" });
                    }
                    context.Comments.AddRange(comments);
                    context.SaveChanges();
                }
            }
        }
    }
}

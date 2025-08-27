using GREPO;

internal class Program
{
    private static void Main(string[] args)
    {
        Console.WriteLine("Init MSSQLXG");
        
        DALMSSQLXG.Init.Execute();

        Console.WriteLine("Start");

        using (DALMSSQLXG.IRepository repo = DALMSSQLXG.Repository.Create())
        {
            repo.getAllWSRef().ForEach(wsRef =>
            {
                Console.WriteLine($"WSRefs: {wsRef.Id}: {wsRef.Url}, {wsRef.Description}, {wsRef.Minus}, {wsRef.Plus}");
            });

            repo.getAllComment().ForEach(comment =>
            {
                Console.WriteLine($"Comments {comment.Id}: {comment.Commtext}, {comment.Stamp}, {comment.WSrefId}");
            });

            if (repo.addWSRef(new DALMSSQLXG.WSRef { Url = "https://www.belstu.by/", Description = "БГТУ", Minus = 0, Plus = 0 }))
                Console.WriteLine("WSRefs: Add");
            else
                Console.WriteLine("WSRefs: Error Add");

            if (repo.addComment(new DALMSSQLXG.Comment { WSrefId = 3, Commtext = "test", Stamp = DateTime.Now }))
                Console.WriteLine("Comments: Add");
            else
                Console.WriteLine("Comments: Error Add");
            if (repo.addComment(new DALMSSQLXG.Comment { WSrefId = 123, Commtext = "test", Stamp = DateTime.Now }))
                Console.WriteLine("Comments: Add");
            else
                Console.WriteLine("Comments: Error Add");

            Console.WriteLine("After addWSRef, addComment");

            repo.getAllWSRef().ForEach(wsRef =>
            {
                Console.WriteLine($"WSRefs: {wsRef.Id}: {wsRef.Url}, {wsRef.Description}, {wsRef.Minus}, {wsRef.Plus}");
            });

            repo.getAllComment().ForEach(comment =>
            {
                Console.WriteLine($"Comments {comment.Id}: {comment.Commtext}, {comment.Stamp}, {comment.WSrefId}");
            });
        }

        Console.WriteLine("Finish MSSQLXG");
        Console.ReadLine();

        Console.WriteLine("Init JSONXG");

        DALJSONXG.Init.Execute();

        Console.WriteLine("Start");

        using (DALJSONXG.IRepository repo = DALJSONXG.Repository.Create())
        {
            repo.getAllWSRef().ForEach(wsRef =>
            {
                Console.WriteLine($"WSRefs: {wsRef.Id}: {wsRef.Url}, {wsRef.Description}, {wsRef.Minus}, {wsRef.Plus}");
            });

            repo.getAllComment().ForEach(comment =>
            {
                Console.WriteLine($"Comments {comment.Id}: {comment.Commtext}, {comment.Stamp}, {comment.WSrefId}");
            });

            if (repo.addWSRef(new DALJSONXG.WSRef { Url = "https://www.belstu.by/", Description = "БГТУ", Minus = 0, Plus = 0 }))
                Console.WriteLine("WSRefs: Add");
            else
                Console.WriteLine("WSRefs: Error Add");

            if (repo.addComment(new DALJSONXG.Comment { WSrefId = 3, Commtext = "test", Stamp = DateTime.Now }))
                Console.WriteLine("Comments: Add");
            else
                Console.WriteLine("Comments: Error Add");
            if (repo.addComment(new DALJSONXG.Comment { WSrefId = 123, Commtext = "test", Stamp = DateTime.Now }))
                Console.WriteLine("Comments: Add");
            else
                Console.WriteLine("Comments: Error Add");

            Console.WriteLine("After addWSRef, addComment");

            repo.getAllWSRef().ForEach(wsRef =>
            {
                Console.WriteLine($"WSRefs: {wsRef.Id}: {wsRef.Url}, {wsRef.Description}, {wsRef.Minus}, {wsRef.Plus}");
            });

            repo.getAllComment().ForEach(comment =>
            {
                Console.WriteLine($"Comments {comment.Id}: {comment.Commtext}, {comment.Stamp}, {comment.WSrefId}");
            });

        }

        Console.WriteLine("Finish JSONXG");
        Console.ReadLine();
    }
}
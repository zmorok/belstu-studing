using REPO;

internal class Program
{
    private static void Main(string[] args)
    {
        Console.WriteLine("Init MSSQL");
        DALMSSQLX.Init.Execute();

        Console.WriteLine("Start MSSQL");
        using (IRepository repo = DALMSSQLX.Repository.Create())
        {
            repo.getAllWSRef().ForEach(wsRef =>
            {
                Console.WriteLine($"WSRefs: {wsRef.Id}: {wsRef.Url}, {wsRef.Description}, {wsRef.Minus}, {wsRef.Plus}");
            });

            repo.getAllComment().ForEach(comment =>
            {
                Console.WriteLine($"Comments {comment.Id}: {comment.Commtext}, {comment.Stamp}, {comment.WSrefId}");
            });

            if (repo.addWSRef(new WSRef() { Url = "https://www.belstu.by/", Description = "БГТУ", Minus = 0, Plus = 0 }))
                Console.WriteLine("WSRefs: Add");
            else
                Console.WriteLine("WSRefs: Error Add");

            if (repo.addComment(new Comment() { WSrefId = 3, Commtext = "test", Stamp = DateTime.Now }))
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

        Console.WriteLine("Finish MSSQL");
        Console.ReadLine();

        Console.WriteLine("Init JSON");
        DALJSONX.Init.Execute();

        Console.WriteLine("Start JSON");
        using (IRepository repo = DALJSONX.Repository.Create())
        {
            repo.getAllWSRef().ForEach(wsRef =>
            {
                Console.WriteLine($"WSRefs: {wsRef.Id}: {wsRef.Url}, {wsRef.Description}, {wsRef.Minus}, {wsRef.Plus}");
            });

            repo.getAllComment().ForEach(comment =>
            {
                Console.WriteLine($"Comments {comment.Id}: {comment.Commtext}, {comment.Stamp}, {comment.WSrefId}");
            });

            if (repo.addWSRef(new WSRef() { Url = "https://www.belstu.by/", Description = "БГТУ", Minus = 0, Plus = 0 }))
                Console.WriteLine("WSRefs: Add");
            else
                Console.WriteLine("WSRefs: Error Add");

            if (repo.addComment(new Comment() { WSrefId = 3, Commtext = "test", Stamp = DateTime.Now }))
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

        Console.WriteLine("Finish MSSQL");
        Console.ReadLine();
    }
}
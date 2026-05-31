namespace ASPA008_1.Helpers
{
    public class FoundByIdException : Exception { public FoundByIdException(string message) : base($"Found by Id: {message}") { } };

    public class SaveException : Exception { public SaveException(string message) : base($"SaveChanges error: {message}") { } }

    public class AddCelebrityException : Exception { public AddCelebrityException(string message) : base($"Add Celebrity Exception error:{message}") { } }
    
    public class UpdCelebrityException : Exception { public UpdCelebrityException(string message) : base($"Update Celebrity Exception error:{message}") { } }

    public class DeleteByIdException : Exception { public DeleteByIdException(string message) : base($"Delete by Id:Delete {message}") { } };

    // new exceptions
    public class NullCelebrityException : Exception { public NullCelebrityException(string message) : base($"Null Celebrity: {message}") { } };

    public class SurnameInvalidException : Exception { public SurnameInvalidException(string message) : base(message) { } };

    public class SurnameConflictException : Exception
    {
        public string Surname { get; }
        public SurnameConflictException(string message, string surname) : base(message) => Surname = surname;
    }
}

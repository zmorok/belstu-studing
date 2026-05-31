using DAL004;

namespace ASPA005_2.Validation
{
    public class PutFilter : IEndpointFilter
    {
        public static IRepository repository = default!;

        public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
        {
            Celebrity? celebrity = context.GetArgument<Celebrity>(1) ?? throw new NullCelebrityException("/Celebrities error, celebrity is null");

            if (celebrity.Surname.Length > 20) throw new SurnameInvalidException("Celebrity.Surname is more than 20");

            return await next(context);
        }
    }
}

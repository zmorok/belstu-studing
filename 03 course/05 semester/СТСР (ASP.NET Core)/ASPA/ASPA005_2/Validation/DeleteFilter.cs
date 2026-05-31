using DAL004;

namespace ASPA005_2.Validation
{
    public class DeleteFilter : IEndpointFilter
    {
        public static IRepository repository = default!;

        public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
        {
            int? id = context.GetArgument<int?>(0);
            if (id is null || id <= 0) throw new DeleteByIdException("Id is null or less than or equal to zero");
            if (id < 20) throw new DeleteByIdException($"Celebrity with Id = {id} [1:20] is not for delete");
            return await next(context);
        }
    }
}

using System.ComponentModel.DataAnnotations;

namespace ApartmentForm
{
    public class INNValidationAttribute : ValidationAttribute
    {
        public INNValidationAttribute()
        {
            ErrorMessage = "ИНН должен состоять из 10 цифр, а сумма цифр должна быть больше 30.";
        }

        public override bool IsValid(object? value)
        {
            if (value is string inn)
            {
                if (inn.Length != 10 || !inn.All(char.IsDigit))
                    return false;

                int sum = inn.Sum(c => c - '0');
                return sum > 30;
            }
            return false;
        }
    }
}

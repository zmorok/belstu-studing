
/*
    Мусорная логиква, чтобы не нагружала код основной формы
 */

namespace ApartmentForm
{
    class ApartmentFormLogic
    {
        // очистка полей адреса
        public void AddressFieldClear()
        {
            ApartmentForm.addressField.Country = "";
            ApartmentForm.addressField.City = "";
            ApartmentForm.addressField.District = "";
            ApartmentForm.addressField.Street = "";
            ApartmentForm.addressField.House = "";
            ApartmentForm.addressField.Subhouse = "";
            ApartmentForm.addressField.ApartmentNumber = "";
        }

        // очистка полей застройщика
        public void DeveloperFieldClear()
        {
            ApartmentForm.developerField.Name = "";
            ApartmentForm.developerField.Type = "";
            ApartmentForm.developerField.Address = "";
            ApartmentForm.developerField.INN = "";
        }

        // вывод сообщения об ошибке
        public void ErrorMessage(string message)
        {
            MessageBox.Show(message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        // вывод информационного сообщения
        public void InfoMessage(string message)
        {
            MessageBox.Show(message, "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        public void WarningMessage(string message)
        {
            MessageBox.Show(message, "Предупреждение", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        // вывод сообщения с вопросом
        public DialogResult QuestionMessage(string message)
        {
            var result = MessageBox.Show(message, "Подтверждение", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            return result;
        }
    }
}

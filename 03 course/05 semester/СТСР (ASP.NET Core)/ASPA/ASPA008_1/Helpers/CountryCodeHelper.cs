using System.Text.Json;

namespace ASPA008_1.Helpers
{
    public class CountryCodes : List<CountryCodes.ISOCountryCodes>
    {
        public record ISOCountryCodes(string code, string countryLabel);
        public CountryCodes(string jsonCountryCodesPath) : base()
        {
            if (File.Exists(jsonCountryCodesPath))
            {
                FileStream fs = new FileStream(jsonCountryCodesPath, FileMode.OpenOrCreate, FileAccess.Read);
                List<ISOCountryCodes>? cc = JsonSerializer.DeserializeAsync<List<ISOCountryCodes>>(fs).Result;
                if (cc != null) this.AddRange(cc);
            }
        }
    }
}

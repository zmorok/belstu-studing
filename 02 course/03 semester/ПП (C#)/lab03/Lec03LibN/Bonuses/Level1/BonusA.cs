namespace Lec03LibN.Bonuses.Level1
{
    public class BonusA : IBonus
    {
        public float cH { get; set; }

        public BonusA(float cH)
        {
            this.cH = cH;
        }

        public float calc(float wH)
        {
            return wH * cH;
        }
    }
}

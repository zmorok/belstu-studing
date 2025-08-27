namespace Lec03LibN.Bonuses.Level2
{
    public class BonusA : IBonus
    {
        public float cH { get; set; }
        public float a { get; set; }

        public BonusA(float cH, float a)
        {
            this.cH = cH;
            this.a = a;
        }

        public float calc(float wH)
        {
            return (wH + a) * cH;
        }
    }
}

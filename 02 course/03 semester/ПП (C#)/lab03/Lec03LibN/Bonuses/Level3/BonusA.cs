namespace Lec03LibN.Bonuses.Level3
{
    public class BonusA : IBonus
    {
        public float cH { get; set; }
        public float a { get; set; }
        public float b { get; set; }

        public BonusA(float cH, float a, float b)
        {
            this.cH = cH;
            this.a = a;
            this.b = b;
        }

        public float calc(float wH)
        {
            return (wH + a) * (cH + b);
        }
    }
}

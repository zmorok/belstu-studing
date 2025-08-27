namespace Lec03LibN.Bonuses.Level3
{
    public class BonusB : IBonus
    {
        public float cH { get; set; }
        public float x { get; set; }
        public float a { get; set; }
        public float b { get; set; }

        public BonusB(float cH, float x, float a, float b)
        {
            this.cH = cH;
            this.x = x;
            this.a = a;
            this.b = b;
        }

        public float calc(float wH)
        {
            return (wH + a) * (cH + b) * x;
        }
    }
}

namespace Lec03LibN.Bonuses.Level3
{
    public class BonusC : IBonus
    {
        public float cH { get; set; }
        public float x { get; set; }
        public float y { get; set; }
        public float a { get; set; }
        public float b { get; set; }

        public BonusC(float cH, float x, float y, float a, float b)
        {
            this.cH = cH;
            this.x = x;
            this.y = y;
            this.a = a;
            this.b = b;
        }

        public float calc(float wH)
        {
            return (wH + a) * (cH + b) * x + y;
        }
    }
}

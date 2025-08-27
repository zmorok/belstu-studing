namespace Lec03LibN.Bonuses.Level2
{
    public class BonusC : IBonus
    {
        public float cH { get; set; }
        public float x { get; set; }
        public float y { get; set; }
        public float a { get; set; }

        public BonusC(float cH, float x, float y, float a)
        {
            this.cH = cH;
            this.x = x;
            this.y = y;
            this.a = a;
        }

        public float calc(float wH)
        {
            return (wH + a) * cH * x + y;
        }
    }
}

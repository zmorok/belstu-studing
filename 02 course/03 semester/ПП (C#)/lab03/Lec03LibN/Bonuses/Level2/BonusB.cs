namespace Lec03LibN.Bonuses.Level2
{
    public class BonusB : IBonus
    {
        public float cH { get; set; }
        public float x { get; set; }
        public float a { get; set; }

        public BonusB(float cH, float x, float a)
        {
            this.cH = cH;
            this.x = x;
            this.a = a;
        }

        public float calc(float wH)
        {
            return (wH + a) * cH * x;
        }
    }
}

namespace Lec03LibN.Bonuses.Level1
{
    public class BonusC : IBonus
    {
        public float cH { get; set; }
        public float x { get; set; }
        public float y { get; set; }

        public BonusC(float cH, float x, float y)
        {
            this.cH = cH;
            this.x = x;
            this.y = y;
        }

        public float calc(float wH)
        {
            return wH * cH * x + y;
        }
    }
}

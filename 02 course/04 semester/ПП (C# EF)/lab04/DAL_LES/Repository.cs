namespace DAL_LES
{
    public class Repository : IRepository
    {
        private readonly Context context;

        private Repository()
        {
            context = new Context();
        }

        public static Repository Create()
        {
            return new Repository(); 
        }

        #region Celebrity (ICelebrity)
        public List<Celebrity> GetAllCelebrities() => context.Celebrities.ToList();

        public Celebrity? GetCelebrityById(int id) => context.Celebrities.FirstOrDefault(c => c.Id == id);

        public bool AddCelebrity(Celebrity celebrity)
        {
            context.Celebrities.Add(celebrity);
            return context.SaveChanges() > 0;
        }

        public bool UpdateCelebrity(int id, Celebrity celebrity)
        {
            var existing = context.Celebrities.Find(id);
            if (existing == null) return false;

            existing.FullName = celebrity.FullName;
            existing.Nationality = celebrity.Nationality;
            existing.ReqPhotoPath = celebrity.ReqPhotoPath;

            return context.SaveChanges() > 0;
        }

        public bool DelCelebrity(int id)
        {
            var celebrity = context.Celebrities.Find(id);
            if (celebrity == null) return false;

            context.Celebrities.Remove(celebrity);
            return context.SaveChanges() > 0;
        }
        #endregion

        #region Lifeevent (ILifeevent)
        public List<Lifeevent> GetAllLifeevents() => context.Lifeevents.ToList();

        public Lifeevent? GetLifeeventById(int id) => context.Lifeevents.FirstOrDefault(l => l.Id == id);

        public bool AddLifeevent(Lifeevent lifeevent)
        {
            if (lifeevent.Date.Kind == DateTimeKind.Unspecified)
                lifeevent.Date = DateTime.SpecifyKind(lifeevent.Date, DateTimeKind.Utc);

            context.Lifeevents.Add(lifeevent);
            return context.SaveChanges() > 0;
        }

        public bool UpdateLifeevent(int id, Lifeevent lifeevent)
        {
            var existing = context.Lifeevents.Find(id);
            if (existing == null) return false;

            existing.Date = lifeevent.Date.Kind == DateTimeKind.Unspecified
                ? DateTime.SpecifyKind(lifeevent.Date, DateTimeKind.Utc)
                : lifeevent.Date;

            existing.Description = lifeevent.Description;
            existing.ReqPhotoPath = lifeevent.ReqPhotoPath;
            existing.CelebrityId = lifeevent.CelebrityId;

            return context.SaveChanges() > 0;
        }

        public bool DelLifeevent(int id)
        {
            var lifeevent = context.Lifeevents.Find(id);
            if (lifeevent == null) return false;

            context.Lifeevents.Remove(lifeevent);
            return context.SaveChanges() > 0;
        }
        #endregion

        #region Common (ICommon)

        public List<Lifeevent> GetLifeeventsByCelebrityId(int celebrityId)
        {
            return context.Lifeevents.Where(l => l.CelebrityId == celebrityId).ToList();
        }

        public Celebrity? GetCelebrityByLifeeventId(int lifeeventId)
        {
            return context.Lifeevents.Where(l => l.Id == lifeeventId).Select(l => l.Celebrity).FirstOrDefault();
        }

        #endregion

        public void Dispose() 
        { 
            context.Dispose(); 
        }
    }
}
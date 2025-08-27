using FreelanceApp.Models;
using System.IO;

namespace FreelanceApp.Services
{
    // class for logging any action, can be expanded in future
    public static class Logger
    {
        #region Filenames
        private static readonly string LogFile = "actions.log";
        private static readonly string FallbackFile = "actions_errors.log";
        #endregion

        #region Main Log
        public static void Log(User user, string action)
        {
            try
            {
                string logEntry = $"{DateTime.Now:G} | {user.Username} ({user.Role}) | - {action}";
                File.AppendAllText(LogFile, logEntry + Environment.NewLine);
            }
            catch (Exception ex)
            {
                File.AppendAllText(FallbackFile,
                    $"{DateTime.Now:G} | ERROR logging action '{action}' for user '{user.Username}': {ex.Message}{Environment.NewLine}");
            }
        }
        #endregion

        #region LogError
        public static void LogError(User user, string action)
        {
            try
            {
                string logEntry = $"{DateTime.Now:G} | {action} | name: {user.Username}; password: {user.Password}";
                File.AppendAllText(LogFile, logEntry + Environment.NewLine);
            }
            catch (Exception ex)
            {
                File.AppendAllText(FallbackFile,
                    $"{DateTime.Now:G} | ERROR logging error_action '{action}' for user '{user.Username}': {ex.Message}{Environment.NewLine}");
            }
        }
        #endregion
    }
}

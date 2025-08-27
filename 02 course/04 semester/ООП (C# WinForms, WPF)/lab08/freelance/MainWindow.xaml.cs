using System;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace freelance
{
    public partial class MainWindow : Window
    {
        private readonly string connectionString =
            ConfigurationManager.ConnectionStrings["FreelanceDB"].ConnectionString;

        // Freelancers
        private DataTable freelancersTable = new DataTable();
        private SqlDataAdapter freelancersAdapter;
        private byte[] currentFreelancerPhoto;

        // Clients
        private DataTable clientsTable = new DataTable();
        private SqlDataAdapter clientsAdapter;
        private byte[] currentClientPhoto;

        // Projects
        private DataTable projectsTable = new DataTable();
        private SqlDataAdapter projectsAdapter;
        private byte[] currentProjectPhoto;

        public MainWindow()
        {
            InitializeComponent();
            LoadFreelancers();
            LoadClients();
            LoadProjects();
        }

        #region Load Methods

        private void LoadFreelancers()
        {
            using var conn = new SqlConnection(connectionString);
            freelancersAdapter = new SqlDataAdapter("SELECT * FROM Freelancers", conn);
            new SqlCommandBuilder(freelancersAdapter);
            freelancersTable.Clear();
            freelancersAdapter.Fill(freelancersTable);
            dgFreelancers.ItemsSource = freelancersTable.DefaultView;
            ClearFreelancerForm();
        }

        private void LoadClients()
        {
            using var conn = new SqlConnection(connectionString);
            clientsAdapter = new SqlDataAdapter("SELECT * FROM Clients", conn);
            new SqlCommandBuilder(clientsAdapter);
            clientsTable.Clear();
            clientsAdapter.Fill(clientsTable);
            dgClients.ItemsSource = clientsTable.DefaultView;
            ClearClientForm();
        }

        private void LoadProjects()
        {
            using var conn = new SqlConnection(connectionString);
            projectsAdapter = new SqlDataAdapter("SELECT * FROM Projects", conn);
            new SqlCommandBuilder(projectsAdapter);
            projectsTable.Clear();
            projectsAdapter.Fill(projectsTable);
            dgProjects.ItemsSource = projectsTable.DefaultView;

            // Fill client ComboBox
            cbProjectClient.ItemsSource = clientsTable.DefaultView;
            cbProjectClient.DisplayMemberPath = "FullName";
            cbProjectClient.SelectedValuePath = "ClientID";

            ClearProjectForm();
        }

        #endregion

        #region Freelancers Tab

        private void LoadFreelancerPhoto_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Images (*.jpg;*.png)|*.jpg;*.png"
            };
            if (dlg.ShowDialog() == true)
            {
                currentFreelancerPhoto = File.ReadAllBytes(dlg.FileName);
                var img = new BitmapImage();
                using var ms = new MemoryStream(currentFreelancerPhoto);
                img.BeginInit();
                img.CacheOption = BitmapCacheOption.OnLoad;
                img.StreamSource = ms;
                img.EndInit();
                imgFreelancerPhoto.Source = img;
            }
        }

        private void AddFreelancer_Click(object sender, RoutedEventArgs e)
        {
            using var conn = new SqlConnection(connectionString);
            conn.Open();
            using var tran = conn.BeginTransaction();
            try
            {
                var cmd = new SqlCommand(
                    @"INSERT INTO Freelancers (FullName, Email, ProfilePhoto)
                      VALUES (@n, @e, @p)",
                    conn, tran);
                cmd.Parameters.AddWithValue("@n", txtFreelancerName.Text);
                cmd.Parameters.AddWithValue("@e", txtFreelancerEmail.Text);
                cmd.Parameters.AddWithValue("@p", (object)currentFreelancerPhoto ?? DBNull.Value);
                cmd.ExecuteNonQuery();
                tran.Commit();
                LoadFreelancers();
            }
            catch (Exception ex)
            {
                tran.Rollback();
                MessageBox.Show("Ошибка при добавлении фрилансера:\n" + ex.Message);
            }
        }

        private void UpdateFreelancer_Click(object sender, RoutedEventArgs e)
        {
            if (dgFreelancers.SelectedItem is not DataRowView row) return;
            int id = (int)row["FreelancerID"];

            using var conn = new SqlConnection(connectionString);
            conn.Open();
            using var tran = conn.BeginTransaction();
            try
            {
                var cmd = new SqlCommand(
                    @"UPDATE Freelancers
                      SET FullName=@n, Email=@e, ProfilePhoto=@p
                      WHERE FreelancerID=@id",
                    conn, tran);
                cmd.Parameters.AddWithValue("@id", id);
                cmd.Parameters.AddWithValue("@n", txtFreelancerName.Text);
                cmd.Parameters.AddWithValue("@e", txtFreelancerEmail.Text);
                cmd.Parameters.AddWithValue("@p", (object)currentFreelancerPhoto ?? DBNull.Value);
                cmd.ExecuteNonQuery();
                tran.Commit();
                LoadFreelancers();
            }
            catch (Exception ex)
            {
                tran.Rollback();
                MessageBox.Show("Ошибка при обновлении фрилансера:\n" + ex.Message);
            }
        }

        private void DeleteFreelancer_Click(object sender, RoutedEventArgs e)
        {
            if (dgFreelancers.SelectedItem is not DataRowView row) return;
            int id = (int)row["FreelancerID"];
            using var conn = new SqlConnection(connectionString);
            conn.Open();
            var cmd = new SqlCommand("DELETE FROM Freelancers WHERE FreelancerID=@id", conn);
            cmd.Parameters.AddWithValue("@id", id);
            cmd.ExecuteNonQuery();
            LoadFreelancers();
        }

        private void SearchFreelancers_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (freelancersTable.Columns.Count == 0) return;
            var f = txtSearchFreelancers.Text.Replace("'", "''");
            freelancersTable.DefaultView.RowFilter =
                $"[FullName] LIKE '%{f}%' OR [Email] LIKE '%{f}%'";
        }

        private void Freelancers_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (dgFreelancers.SelectedItem is DataRowView row)
            {
                txtFreelancerName.Text = row["FullName"].ToString();
                txtFreelancerEmail.Text = row["Email"].ToString();

                if (row["ProfilePhoto"] != DBNull.Value)
                {
                    var data = (byte[])row["ProfilePhoto"];
                    currentFreelancerPhoto = data;
                    var img = new BitmapImage();
                    using var ms = new MemoryStream(data);
                    img.BeginInit();
                    img.CacheOption = BitmapCacheOption.OnLoad;
                    img.StreamSource = ms;
                    img.EndInit();
                    imgFreelancerPhoto.Source = img;
                }
                else
                {
                    imgFreelancerPhoto.Source = null;
                    currentFreelancerPhoto = null;
                }
            }
        }

        private void ClearFreelancerForm()
        {
            txtFreelancerName.Clear();
            txtFreelancerEmail.Clear();
            imgFreelancerPhoto.Source = null;
            currentFreelancerPhoto = null;
            txtSearchFreelancers.Clear();
        }

        #endregion

        #region Clients Tab

        private void LoadClientPhoto_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Images (*.jpg;*.png)|*.jpg;*.png"
            };
            if (dlg.ShowDialog() == true)
            {
                currentClientPhoto = File.ReadAllBytes(dlg.FileName);
                var img = new BitmapImage();
                using var ms = new MemoryStream(currentClientPhoto);
                img.BeginInit();
                img.CacheOption = BitmapCacheOption.OnLoad;
                img.StreamSource = ms;
                img.EndInit();
                imgClientPhoto.Source = img;
            }
        }

        private void AddClient_Click(object sender, RoutedEventArgs e)
        {
            using var conn = new SqlConnection(connectionString);
            conn.Open();
            using var tran = conn.BeginTransaction();
            try
            {
                var cmd = new SqlCommand(
                    @"INSERT INTO Clients (FullName, Email, ClientPhoto)
                      VALUES (@n, @e, @p)",
                    conn, tran);
                cmd.Parameters.AddWithValue("@n", txtClientName.Text);
                cmd.Parameters.AddWithValue("@e", txtClientEmail.Text);
                cmd.Parameters.AddWithValue("@p", (object)currentClientPhoto ?? DBNull.Value);
                cmd.ExecuteNonQuery();
                tran.Commit();
                LoadClients();
                LoadProjects();  // обновляем проекты, т.к. список клиентов мог измениться
            }
            catch (Exception ex)
            {
                tran.Rollback();
                MessageBox.Show("Ошибка при добавлении клиента:\n" + ex.Message);
            }
        }

        private void UpdateClient_Click(object sender, RoutedEventArgs e)
        {
            if (dgClients.SelectedItem is not DataRowView row) return;
            int id = (int)row["ClientID"];

            using var conn = new SqlConnection(connectionString);
            conn.Open();
            using var tran = conn.BeginTransaction();
            try
            {
                var cmd = new SqlCommand(
                    @"UPDATE Clients
                      SET FullName=@n, Email=@e, ClientPhoto=@p
                      WHERE ClientID=@id",
                    conn, tran);
                cmd.Parameters.AddWithValue("@id", id);
                cmd.Parameters.AddWithValue("@n", txtClientName.Text);
                cmd.Parameters.AddWithValue("@e", txtClientEmail.Text);
                cmd.Parameters.AddWithValue("@p", (object)currentClientPhoto ?? DBNull.Value);
                cmd.ExecuteNonQuery();
                tran.Commit();
                LoadClients();
                LoadProjects();
            }
            catch (Exception ex)
            {
                tran.Rollback();
                MessageBox.Show("Ошибка при обновлении клиента:\n" + ex.Message);
            }
        }

        private void DeleteClient_Click(object sender, RoutedEventArgs e)
        {
            if (dgClients.SelectedItem is not DataRowView row) return;
            int id = (int)row["ClientID"];
            using var conn = new SqlConnection(connectionString);
            conn.Open();
            var cmd = new SqlCommand("DELETE FROM Clients WHERE ClientID=@id", conn);
            cmd.Parameters.AddWithValue("@id", id);
            cmd.ExecuteNonQuery();
            LoadClients();
            LoadProjects();
        }

        private void SearchClients_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (clientsTable.Columns.Count == 0) return;
            var f = txtSearchClients.Text.Replace("'", "''");
            clientsTable.DefaultView.RowFilter =
                $"[FullName] LIKE '%{f}%'";
        }

        private void Clients_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (dgClients.SelectedItem is DataRowView row)
            {
                txtClientName.Text = row["FullName"].ToString();
                txtClientEmail.Text = row["Email"].ToString();

                if (row["ClientPhoto"] != DBNull.Value)
                {
                    var data = (byte[])row["ClientPhoto"];
                    currentClientPhoto = data;
                    var img = new BitmapImage();
                    using var ms = new MemoryStream(data);
                    img.BeginInit();
                    img.CacheOption = BitmapCacheOption.OnLoad;
                    img.StreamSource = ms;
                    img.EndInit();
                    imgClientPhoto.Source = img;
                }
                else
                {
                    imgClientPhoto.Source = null;
                    currentClientPhoto = null;
                }
            }
        }

        private void ClearClientForm()
        {
            txtClientName.Clear();
            txtClientEmail.Clear();
            imgClientPhoto.Source = null;
            currentClientPhoto = null;
            txtSearchClients.Clear();
        }

        #endregion

        #region Projects Tab

        private void LoadProjectPhoto_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Images (*.jpg;*.png)|*.jpg;*.png"
            };
            if (dlg.ShowDialog() == true)
            {
                currentProjectPhoto = File.ReadAllBytes(dlg.FileName);
                var img = new BitmapImage();
                using var ms = new MemoryStream(currentProjectPhoto);
                img.BeginInit();
                img.CacheOption = BitmapCacheOption.OnLoad;
                img.StreamSource = ms;
                img.EndInit();
                imgProjectPhoto.Source = img;
            }
        }

        private void AddProject_Click(object sender, RoutedEventArgs e)
        {
            using var conn = new SqlConnection(connectionString);
            conn.Open();
            using var tran = conn.BeginTransaction();
            try
            {
                var cmd = new SqlCommand(
                    @"INSERT INTO Projects (ClientID, Title, Budget, ProjectPhoto)
                      VALUES (@c, @t, @b, @p)",
                    conn, tran);
                cmd.Parameters.AddWithValue("@c", cbProjectClient.SelectedValue);
                cmd.Parameters.AddWithValue("@t", txtProjectTitle.Text);
                cmd.Parameters.AddWithValue("@b", decimal.Parse(txtProjectBudget.Text));
                cmd.Parameters.AddWithValue("@p", (object)currentProjectPhoto ?? DBNull.Value);
                cmd.ExecuteNonQuery();
                tran.Commit();
                LoadProjects();
            }
            catch (Exception ex)
            {
                tran.Rollback();
                MessageBox.Show("Ошибка при добавлении проекта:\n" + ex.Message);
            }
        }

        private void UpdateProject_Click(object sender, RoutedEventArgs e)
        {
            if (dgProjects.SelectedItem is not DataRowView row) return;
            int id = (int)row["ProjectID"];

            using var conn = new SqlConnection(connectionString);
            conn.Open();
            using var tran = conn.BeginTransaction();
            try
            {
                var cmd = new SqlCommand(
                    @"UPDATE Projects
                      SET ClientID=@c, Title=@t, Budget=@b, ProjectPhoto=@p
                      WHERE ProjectID=@id",
                    conn, tran);
                cmd.Parameters.AddWithValue("@id", id);
                cmd.Parameters.AddWithValue("@c", cbProjectClient.SelectedValue);
                cmd.Parameters.AddWithValue("@t", txtProjectTitle.Text);
                cmd.Parameters.AddWithValue("@b", decimal.Parse(txtProjectBudget.Text));
                cmd.Parameters.AddWithValue("@p", (object)currentProjectPhoto ?? DBNull.Value);
                cmd.ExecuteNonQuery();
                tran.Commit();
                LoadProjects();
            }
            catch (Exception ex)
            {
                tran.Rollback();
                MessageBox.Show("Ошибка при обновлении проекта:\n" + ex.Message);
            }
        }

        private void DeleteProject_Click(object sender, RoutedEventArgs e)
        {
            if (dgProjects.SelectedItem is not DataRowView row) return;
            int id = (int)row["ProjectID"];
            using var conn = new SqlConnection(connectionString);
            conn.Open();
            var cmd = new SqlCommand("DELETE FROM Projects WHERE ProjectID=@id", conn);
            cmd.Parameters.AddWithValue("@id", id);
            cmd.ExecuteNonQuery();
            LoadProjects();
        }

        private void SearchProjects_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (projectsTable.Columns.Count == 0) return;
            var f = txtSearchProjects.Text.Replace("'", "''");
            projectsTable.DefaultView.RowFilter =
                $"[Title] LIKE '%{f}%'";
        }

        private void Projects_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (dgProjects.SelectedItem is DataRowView row)
            {
                cbProjectClient.SelectedValue = row["ClientID"];
                txtProjectTitle.Text = row["Title"].ToString();
                txtProjectBudget.Text = row["Budget"].ToString();

                if (row["ProjectPhoto"] != DBNull.Value)
                {
                    var data = (byte[])row["ProjectPhoto"];
                    currentProjectPhoto = data;
                    var img = new BitmapImage();
                    using var ms = new MemoryStream(data);
                    img.BeginInit();
                    img.CacheOption = BitmapCacheOption.OnLoad;
                    img.StreamSource = ms;
                    img.EndInit();
                    imgProjectPhoto.Source = img;
                }
                else
                {
                    imgProjectPhoto.Source = null;
                    currentProjectPhoto = null;
                }
            }
        }

        private void ClearProjectForm()
        {
            cbProjectClient.SelectedIndex = -1;
            txtProjectTitle.Clear();
            txtProjectBudget.Clear();
            imgProjectPhoto.Source = null;
            currentProjectPhoto = null;
            txtSearchProjects.Clear();
        }

        #endregion
    }
}

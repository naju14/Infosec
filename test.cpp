https://github.com/naju14/Infosec.git

using System;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Windows.Forms;

namespace VuurwerkKassa
{
    public partial class MainGUI : Form
    {
        private DBConnection db = new DBConnection();
        private Model m = new Model();
        private Boolean klantViewRunning = false;
        private KlantView klantView;
        private ListView listViewKlant;
        private Label lblTotaalKlant;

        public MainGUI()
        {
            InitializeComponent();
        }

        private void MainGUI_Load(object sender, EventArgs e)
        {
            listView1.Items.Clear();
            lblTotaal.Text = 0.ToString("c2");
            db.openConnection();
            txtBARcode.Focus();
        }

        private void MainGUI_Click(object sender, EventArgs e)
        {
            txtBARcode.Focus();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            txtBARcode.Focus();
        }

        private void lblTotaal_Click(object sender, EventArgs e)
        {
            txtBARcode.Focus();
        }

        private void lblCredits_Click(object sender, EventArgs e)
        {
            txtBARcode.Focus();
        }

        private void txtBARcode_TextChanged(object sender, EventArgs e)
        {
            String txt = txtBARcode.Text;
            if (txt.Length > 0 && txt[txt.Length - 1] == '\n')
            {
                txt = txt.Substring(0, txt.Length - 2);
                m.addArtikel(db.getArtikelByBarCode(txt));
                updateListView();
                txtBARcode.Clear();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult result = MsgBoxes.DialogBox("Afsluiten?", "De applicatie wordt beëindigd.\nZeker weten?");
            if (result == DialogResult.Yes)
            {
                db.closeConnection();
                Application.Exit();
            }
            txtBARcode.Focus();
        }

        private void btnNieuweKlant_Click(object sender, EventArgs e)
        {
            DialogResult result = MsgBoxes.DialogBox("Volgende Klant?", "Het huidige overzicht wordt gereset.\nZeker weten?");
            if (result == DialogResult.Yes)
            {
                m.resetArtikelen();
                listView1.Items.Clear();
                lblTotaal.Text = 0.ToString("c2");
                if (klantViewRunning)
                {
                    listViewKlant.Items.Clear();
                    lblTotaalKlant.Text = 0.ToString("c2");
                }
            }
            txtBARcode.Focus();
        }

        private void btnCash_Click(object sender, EventArgs e)
        {
            string verm = m.getGrandTotal().ToString();
            DialogResult result = MsgBoxes.InputBox("Hoeveel Cash?", "Met hoeveel heeft de kant betaald?", ref verm);
            if (result == DialogResult.OK)
            {
                float cash1 = float.Parse(verm, new CultureInfo("en-US"));
                float cash2 = float.Parse(verm, new CultureInfo("nl-NL"));
                float cash = (cash1 > cash2 ? cash2 : cash1);
                float change = cash - m.getGrandTotal();
                MsgBoxes.DialogBoxOk("Wisselgeld...", "Het wisselgeld is:\n\n            " + change.ToString("c2"));
            }
            txtBARcode.Focus();
        }

        private void btnPin_Click(object sender, EventArgs e)
        {
            string verm = "0";
            DialogResult result = MsgBoxes.InputBox("Hoeveel Cash?", "Voor hoeveel wil de klant met Cash betalen?", ref verm);
            if (result == DialogResult.OK)
            {
                float cash1 = float.Parse(verm, new CultureInfo("en-US"));
                float cash2 = float.Parse(verm, new CultureInfo("nl-NL"));
                float pin = m.getGrandTotal() - (cash1 > cash2 ? cash2 : cash1);
                MsgBoxes.DialogBoxOk("Nog te Pinnen...", "Het bedrag nog te pinnen is:\n\n            " + pin.ToString("c2"));
            }
            txtBARcode.Focus();
        }

        private void btnVeranderPrijs_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count == 1)
            {
                int selectedIdx = listView1.SelectedIndices[0];
                Artikel art = m.getArtikelen()[selectedIdx];
                string verm = art.getPrijs().ToString();
                DialogResult result = MsgBoxes.InputBox("Nieuwe prijs?", "Wat is de nieuwe artikelprijs?", ref verm);
                if (result == DialogResult.OK)
                {
                    float cash1 = float.Parse(verm, new CultureInfo("en-US"));
                    float cash2 = float.Parse(verm, new CultureInfo("nl-NL"));
                    float cash = (cash1 > cash2 ? cash2 : cash1);
                    art.setPrijs(cash);
                    updateListView();
                }
            }
            txtBARcode.Focus();
        }

        private void btnVeranderTotaal_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count == 1)
            {
                int selectedIdx = listView1.SelectedIndices[0];
                Artikel art = m.getArtikelen()[selectedIdx];
                string verm = art.getTotaal().ToString();
                DialogResult result = MsgBoxes.InputBox("Nieuwe totaalprijs?", "Wat is de nieuwe totaalprijs?", ref verm);
                if (result == DialogResult.OK)
                {
                    float cash1 = float.Parse(verm, new CultureInfo("en-US"));
                    float cash2 = float.Parse(verm, new CultureInfo("nl-NL"));
                    float cash = (cash1 > cash2 ? cash2 : cash1);
                    art.setTotaal(cash);
                    updateListView();
                }
            }
            else
            {
                string verm = m.getGrandTotal().ToString();
                DialogResult result = MsgBoxes.InputBox("Nieuwe totaalprijs?", "Wat is de nieuwe totaalprijs?", ref verm);
                if (result == DialogResult.OK)
                {
                    float cash1 = float.Parse(verm, new CultureInfo("en-US"));
                    float cash2 = float.Parse(verm, new CultureInfo("nl-NL"));
                    float cash = (cash1 > cash2 ? cash2 : cash1);
                    m.setGrandTotal(cash);
                    lblTotaal.Text = m.getGrandTotal().ToString("c2");
                    lblTotaalKlant.Text = m.getGrandTotal().ToString("c2");
                }
            }
            txtBARcode.Focus();
        }

        private void btnVeranderAantal_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count == 1)
            {
                int selectedIdx = listView1.SelectedIndices[0];
                Artikel art = m.getArtikelen()[selectedIdx];
                string verm = art.getAantal().ToString();
                DialogResult result = MsgBoxes.InputBox("Nieuw aantal?", "Wat is het nieuwe artikelaantal?", ref verm);
                if (result == DialogResult.OK)
                {
                    art.setAantal(int.Parse(verm));
                    updateListView();
                }
            }
            txtBARcode.Focus();
        }

        private void btnVerwijder_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count == 1)
            {
                int selectedIdx = listView1.SelectedIndices[0];
                DialogResult result = MsgBoxes.DialogBox("Verwijderen?", "Het artikel wordt verwijderd.\nZeker weten?");
                if (result == DialogResult.Yes)
                {
                    m.removeFromArtikelen(selectedIdx);
                    updateListView();
                }
            }
            txtBARcode.Focus();
        }

        private void btnHandArtOm_Click(object sender, EventArgs e)
        {
            string code = "";
            DialogResult result = MsgBoxes.InputBox("Omschrijving", "Vul (een deel van) de omschrijving in van het toe te voegen artikel...", ref code);
            if (result == DialogResult.OK)
            {
                Collection<Artikel> artikelen = db.getArtikelen(code);
                int selectedIdx = -1;
                result = MsgBoxes.ListBox("Resultaten", "Selecteer een van onderstaande resultaten...", ref selectedIdx, ref artikelen);
                if (result == DialogResult.OK && selectedIdx != -1)
                {
                    m.addArtikel(artikelen[selectedIdx]);
                    updateListView();
                }
            }
            txtBARcode.Focus();
        }

        private void btnHandArtNr_Click(object sender, EventArgs e)
        {
            string code = "";
            DialogResult result = MsgBoxes.InputBox("Artikelnummer", "Vul het nummer in van het toe te voegen artikel...", ref code);
            if (result == DialogResult.OK)
            {
                m.addArtikel(db.getArtikelByArtNr(code));
                updateListView();
            }
            txtBARcode.Focus();
        }

        private void btnHandBarCode_Click(object sender, EventArgs e)
        {
            string code = "";
            DialogResult result = MsgBoxes.InputBox("BAR-Code", "Vul de BAR-Code in van het toe te voegen artikel...", ref code);
            if (result == DialogResult.OK)
            {
                m.addArtikel(db.getArtikelByBarCode(code));
                updateListView();
            }
            txtBARcode.Focus();
        }

        private void btnMultiplicity_Click(object sender, EventArgs e)
        {
            string verm = "1";
            DialogResult result = MsgBoxes.InputBox("Vermenigvuldiging", "Vul het aantal in van het volgende artikel...", ref verm);
            if (result == DialogResult.OK && int.Parse(verm) > 0)
            {
                m.setVermenigvuldiging(int.Parse(verm));
            }
            txtBARcode.Focus();
        }

        private void btnKlantView_Click(object sender, EventArgs e)
        {
            if (!klantViewRunning)
            {
                klantView = new KlantView();
                klantView.Show(this);
                listViewKlant = klantView.getListView();
                lblTotaalKlant = klantView.getTotaalLabel();
                klantViewRunning = true;
            }
            txtBARcode.Focus();
        }

        public void updateListView()
        {
            listView1.Items.Clear();
            if (klantViewRunning)
            {
                listViewKlant.Items.Clear();
            }
            m.setGrandTotal(0);
            foreach (Artikel a in m.getArtikelen())
            {
                m.setGrandTotal(m.getGrandTotal() + a.getTotaal());
                ListViewItem lvi = new ListViewItem(a.getNr().ToString());
                lvi.SubItems.Add(a.getOmsch());
                lvi.SubItems.Add(a.getAantal().ToString());
                lvi.SubItems.Add(a.getPrijs().ToString("0.00"));
                lvi.SubItems.Add(a.getTotaal().ToString("0.00"));
                lvi.Group = listView1.Groups[0];
                listView1.Items.Add(lvi);
                listView1.Items[listView1.Items.Count - 1].EnsureVisible();
                if (klantViewRunning)
                {
                    ListViewItem lviClone = (ListViewItem)lvi.Clone();
                    lviClone.Group = listViewKlant.Groups[0];
                    listViewKlant.Items.Add(lviClone);
                    listViewKlant.Items[listViewKlant.Items.Count - 1].EnsureVisible();
                }
            }
            lblTotaal.Text = m.getGrandTotal().ToString("c2");
            if (klantViewRunning)
            {
                lblTotaalKlant.Text = m.getGrandTotal().ToString("c2");
            }
        }

    }
}
const puppeteer = require('puppeteer');
const fs = require('fs');
const path = require('path');

// Get input/output paths from command line arguments
const [,, inputHtml, outputPdf] = process.argv;

if (!inputHtml || !outputPdf) {
  console.error('Usage: node convert-to-pdf.js <input.html> <output.pdf>');
  process.exit(1);
}

(async () => {
  const htmlPath = path.resolve(__dirname, inputHtml);
  const pdfPath = path.resolve(__dirname, outputPdf);

  const browser = await puppeteer.launch({
    headless: 'new',
    args: ['--no-sandbox', '--disable-setuid-sandbox']
  });

  const page = await browser.newPage();
  const content = fs.readFileSync(htmlPath, 'utf8');
  await page.setContent(content, { waitUntil: 'networkidle0' });

  await page.pdf({
    path: pdfPath,
    format: 'A4',
    printBackground: true,
    margin: { top: '1cm', bottom: '1cm', left: '1cm', right: '1cm' }
  });

  await browser.close();
})();


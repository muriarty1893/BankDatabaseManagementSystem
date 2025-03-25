# User Experience Design 2. Ödev (Ceylin - Murat - Mehmed Emin)
# Çiftlik Bank Veritabanı Yönetim Sistemi
![Mehmet Aydın](edited.jpg)
## Genel Bakış
Bu proje, **Çiftlik Bank Veritabanı Yönetim Sistemi** olarak tasarlanmıştır ve `icb_gui` kütüphanesi kullanılarak geliştirilmiştir. Kullanıcılar, **müşteri hesaplarını kaydedebilir, arayabilir, silebilir ve yönetebilir**. Sistem, hesap numarası, TC kimlik numarası, ad-soyad, bakiye ve fotoğraf gibi bilgileri saklar.

## Özellikler
- **Müşteri hesabı ekleme:** Ad, bakiye ve fotoğraf dahil olmak üzere müşteri bilgilerini saklar.
- **Hesap arama:** Hesap numarasına göre müşteri bilgilerini getirir.
- **Hesap silme:** Belirtilen hesabı veritabanından kaldırır.
- **Fotoğraf yükleme:** Müşteri fotoğraflarını yükleyip görüntüler.
- **Kalıcı veri saklama:** `bank_index.dat` ve `bank_data.dat` dosyaları kullanılarak bilgiler kaydedilir.

## Kullanım
1. **Kaydet:** Yeni bir müşteri kaydı oluşturun.
2. **Ara:** Hesap numarasına göre müşteri bilgilerini getirin.
3. **Sil:** Müşteri kaydını veritabanından kaldırın.
4. **Fotoğraf Yükle:** Müşteri fotoğrafını ekleyin ve görüntüleyin.
5. **Temizle:** Tüm giriş alanlarını sıfırlayın.

## Geliştiriciler İçin
- **Veritabanı Yönetimi:** `BANK_DATABASE` yapısı müşteri verilerini saklar.
- **Dosya Okuma/Yazma:** `WriteICBYTES` ve `ReadICBYTES` fonksiyonları kullanılarak veriler saklanır.
- **Arayüz Yönetimi:** `ICGUI_main()` fonksiyonu, butonları ve giriş alanlarını oluşturur.

## Katkıda Bulunma
Projeye katkıda bulunmak için **pull request açabilir** veya hata bildiriminde bulunabilirsiniz.

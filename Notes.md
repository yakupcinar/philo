***PHILO KLASORÜ İÇERİSİNDE VERİLECEK İNDİĞİNDE PHİLO KLASÖRÜ VE YANINDA READ.ME OLUCAK (KOD PROJE FİLO İÇİNDE OLACAK)

***uslepp mikro saniye bekletir, senin ft_usleep ile onu milisaniyeye çevirip kullanmam lazım o yüzden get_current_time


1. Thread ve Process Farkı
Soru: Önceki projelerde (Pipex, Minishell) fork() kullanarak process oluşturuyorduk. Bu projede neden pthread_create ile thread kullandık? Farkları nedir?

Cevap: fork() ile oluşturulan process'ler tamamen izole çalışır; her birinin kendine ait ayrı bir bellek alanı vardır. Bir process içindeki değişkeni değiştirirse diğeri bunu göremez. Haberleşmek için pipe veya sinyaller gibi özel araçlara ihtiyaç duyarlar.
Thread'ler (iş parçacıkları) ise aynı process'in içinde yaşarlar ve ortak bellek alanını (heap ve data segment) paylaşırlar. Filozoflar masada aynı çatalları kullanmak, birbirlerinin yemeğe başlayıp başlamadığını veya masada birinin ölüp ölmediğini (is_dead bayrağı) anında görmek zorundadır. Bu paylaşımlı anlık hafızaya ihtiyacımız olduğu için projede thread kullanıyoruz.

2. Mutex ve Data Race (Veri Yarışması)
Soru: Neden Mutex (kilit) kullanmak zorundayız? Data Race nedir ve projende neleri korumaya aldın?

Cevap: Thread'ler aynı belleği paylaştığı için, iki filozof (thread) aynı anda aynı çatala uzanmaya veya aynı değişkene (örneğin ölüm bayrağına) değer yazmaya çalışırsa bellek bozulur. Buna Data Race (Veri Yarışı) denir.
Mutex (Mutual Exclusion), bir kaynağı bir thread kullanırken diğerlerinin beklemesini sağlayan bir kilit mekanizmasıdır. Projemde üç kritik şeyi Mutex ile korudum:

Çatallar: İki kişi aynı çatala aynı anda erişemesin diye.

Yazdırma (printf): Ekrana log basarken yazılar birbirine girmesin ve biri öldükten sonra başka çıktı basılmasın diye (print_mutex).

Durum Değişkenleri: Filozofun son yeme zamanı (last_meal_time) veya ölüm durumu (is_dead), biri tarafından okunurken diğeri tarafından değiştirilmesin diye (dead_mutex).

3. Deadlock (Ölümcül Kilitlenme) Çözümü
Soru: Deadlock nedir? Senin projende filozofların kilitlenmesini engelleyen sistem nasıl çalışıyor?

Cevap: Deadlock, masadaki tüm filozofların aynı anda sadece sol çatallarını alıp, sağ çatalları için sonsuza kadar birbirlerini beklemesi (kilitlenmesi) durumudur. Bu durumda kimse yemek yiyemez ve ölürler.
Ben bu sorunu Başlangıç Gecikmesi (Offset) mantığıyla çözdüm. routine fonksiyonumun en başında, çift numaralı filozofları (2, 4, 6...) sadece 15 milisaniye uyutuyorum (ft_usleep(15)). Böylece masaya ilk oturduklarında tek numaralı filozoflar (1, 3, 5...) hiçbir engele takılmadan her iki çatalı da anında alıp yemeğe başlıyor. Başlangıçtaki o trafik sıkışıklığını (yarışmayı) önlediğim için sistem sonsuza kadar kusursuz bir sırayla (senkronizasyonla) dönüyor.

4. Özel Zaman Yönetimi (ft_usleep)
Soru: Neden C'nin kendi standart usleep fonksiyonunu kullanmadın da kendi uyku fonksiyonunu yazdın?

Cevap: İşletim sisteminin standart usleep fonksiyonu "kesinlik" (precision) garantisi vermez. İşletim sistemi CPU'da başka işlemler yapıyorsa, usleep(200) dediğim bir thread 210ms veya 220ms sonra uyanabilir. 42 projelerinde 10ms'lik bir sapma bile ölümle sonuçlanıp testten bırakır.
Bu yüzden kendi ft_usleep fonksiyonumu yazdım. Uykuya başlanılan zamanı alıyorum ve get_current_time ile sürekli kontrol eden mikro bir döngü kuruyorum. CPU'yu tamamen kitlememek için içerisine çok küçük bir bekleme (usleep(500) mikrosaniye) koyuyorum. Bu sayede milisaniyesi milisaniyesine uyanmalarını sağlıyorum.

5. Monitor (Gözlemci) Mantığı
Soru: Neden filozoflar kendi döngüleri içinde kendi ölümlerini kontrol etmiyorlar da ayrı bir monitor (gözlemci) fonksiyonu veya thread'i yazdın?

Cevap: Çünkü bir filozof ft_usleep fonksiyonuyla uyurken veya yemek yerken thread o satırda bekler, alt satırlardaki kodları çalıştıramaz. Diyelim ki filozof uyurken ölüm süresi (time_to_die) doldu; ancak uyanana kadar bunu fark edemez. Bu da "Ölüm en fazla 10ms gecikebilir" kuralının ihlal edilmesine sebep olur.
Bu sorunu çözmek için masaya dışarıdan bakan ve uyumayan ayrı bir monitor_routine sistemi kurdum. Gözlemci sürekli olarak tüm filozofları tarayıp "Son yemeğinden bu yana ölüm süren geçmiş mi?" diye kilitler (mutex) eşliğinde kontrol ediyor. Eğer biri sınırda ölmüşse, o filozof daha uykusundan uyanamadan gözlemci is_dead = 1 bayrağını çekip simülasyonu anında bitiriyor.

İşte -pthread flag'inin tam olarak yaptığı işler:
1. Kütüphaneyi Linklemek (-lpthread)
C dilinde pthread_create, pthread_mutex_lock gibi fonksiyonları kullanıyorsun ancak bu fonksiyonlar standart C kütüphanesinin (libc) doğrudan bir parçası değildir; sistemde ayrı bir kütüphane olan libpthread içinde tanımlıdırlar. -pthread bayrağı, derleyiciye şu komutu verir: "Programı oluştururken libpthread kütüphanesini de projeye dahil et (linkle)."

2. Thread-Safe (İş Parçacığı Güvenli) Derleme
Bu flag sadece bir kütüphane eklemez, aynı zamanda derleyiciye "bu program çoklu iş parçacığıyla çalışacak, buna göre derle" sinyalini gönderir.

Bazı standart C kütüphanesi fonksiyonları (errno değişkeni gibi), tek thread'li bir yapıda farklı, çok thread'li bir yapıda farklı davranmak zorundadır.

-pthread kullanıldığında, derleyici bu fonksiyonları thread-safe sürümleriyle değiştirir (örneğin, errno artık her thread için ayrı bir bellek adresinde tutulur).

2. Neden long long Tercih Ediliyor?
Sadece long yazmak yerine long long (veya direkt uint64_t / int64_t) yazılmasının iki temel sebebi vardır:

Garantiye Almak (Cross-Platform Güvenliği): Kodunun derlendiği bilgisayar (Mac, Linux, Windows fark etmeksizin) neresi olursa olsun, long long ifadesinin kesin olarak 64-bit (8 Byte) yer kaplayacağı C standartlarınca garanti edilmiştir. Savunmada (evaluation) kodunun beklenmedik bir işletim sistemi konfigürasyonunda patlamasını istemezsin.

Mikrosaniye Hesapları: gettimeofday fonksiyonu bize zamanı mikrosaniye (tv_usec) cinsinden de verir. Mikrosaniyeler çok hızlı büyür. 32-bitlik bir sınır (yaklaşık 2.147.483.647), mikrosaniye cinsinden hesaplandığında sadece 35 dakika içinde dolup taşar! Eğer kodun bir yerinde yanlışlıkla milisaniyeyi mikrosaniyeye çevirip 32-bitlik bir değişkene (long veya int) eşitlersen, simülasyonun 35. dakikada çökecektir. long long kullanarak bu süreyi neredeyse sonsuza (yaklaşık 300 bin yıla) çıkarıyoruz.


1. usleep Sadece Bir "Ağrı Kesici"dir
Çift numaralı filozofları başlangıçta (veya döngü içinde) usleep ile uyutmak, simülasyonun ilk birkaç saniyesinde harika çalışır. Trafik sıkışıklığını önler ve tek numaralıların rahatça yemeğe başlamasını sağlar.

Sorun: Ancak usleep (veya ft_usleep) hiçbir zaman %100 hassas değildir. İşletim sistemi (CPU Scheduler), arka planda çalışan diğer programlara da kaynak ayırdığı için senin thread'ini tam vaktinde uyandırmayabilir.

Sonuç: Simülasyon dakikalarca çalıştığında (özellikle süreler birbirine çok yakınsa, örn: 410 200 200), o başlarda kurduğun kusursuz "tekler yer, çiftler bekler" ritmi yavaş yavaş bozulmaya ve kaymaya başlar.

2. Ölümcül An: Kilitlenme (Deadlock)
Ritmin bozulduğu o an geldiğinde (diyelim ki simülasyonun 45. saniyesinde), masadaki 5 filozofun 5'i de tamamen aynı milisaniyede acıkıp masaya uzanabilir.

Eğer asimetrik sıralama yoksa (yani herkes önce sol, sonra sağ çatalı alıyorsa):

5 filozof da aynı anda sol çatalı alır.

Herkes sağ çatalı almak için beklemeye başlar.

Sağ çatallar başkalarının sol elinde olduğu için kimse sağ çatalı bulamaz.

Kimse elindeki çatalı bırakmaz ve herkes açlıktan ölür (Deadlock).

3. Asimetrik Çatal Alma "Gerçek Tedavi"dir
Asimetrik çatal alma (örneğin filozofların her zaman numarası küçük olan çatalı önce alması veya teklerin önce sol, çiftlerin önce sağ çatalı alması), masadaki bu ölüm çemberini (circular wait) fiziksel olarak kırar.

Diyelim ki ritim bozuldu ve herkes aynı milisaniyede çatal almaya çalıştı:

4 filozof birinci kurala uyup sol çatalı alır.

Ancak asimetrik kural sayesinde 5. filozof (veya ID'si farklı olan biri) sol yerine sağ çatalı almaya çalışır.

Sağ çatal zaten 1. filozofun elinde olduğu için 5. filozof hiçbir çatal alamaz ve bekler.

Bu sayede 5. filozofun sol çatalı masada boş kalır! 4. filozof bu boş çatalı alır, yemeğini yer, bırakır ve kilitlenme (deadlock) matematiksel olarak imkansız hale gelir.

1. gettimeofday Bize Ne Döndürür?
C'de zamanı öğrenmek için <sys/time.h> kütüphanesindeki gettimeofday fonksiyonunu kullanırız. Bu fonksiyon bizden struct timeval adında bir yapı (struct) ister ve içini şu iki değerle doldurur:

tv_sec: 1 Ocak 1970'ten bu yana geçen saniyeler.

tv_usec: Geriye kalan küsurat, yani mikrosaniyeler.

Philo subject'i bizden her şeyi milisaniye (ms) cinsinden hesaplamamızı ister. Bu yüzden bu iki değeri birleştirip milisaniyeye çevirmeliyiz:

Saniyeyi milisaniyeye çevirmek için 1000 ile çarparız.

Mikrosaniyeyi milisaniyeye çevirmek için 1000'e böleriz.

2. Neden Normal usleep Yetmiyor? (Eval Sorusu!)
Normal usleep fonksiyonu mikrosaniye cinsinden çalışır. Yani usleep(200) yazarsan, programı 200 milisaniye değil, 200 mikrosaniye (çok çok daha kısa) uyutur. Milisaniye uyutmak için sayıyı 1000 ile çarpıp usleep(time * 1000) yapabilirsin.

Peki 42 bizden neden kendi ft_usleep'imizi yazmamızı istiyor? Çünkü standart usleep tembeldir ve saati şaşar.
Eğer işletim sistemine usleep(200000) (200 ms) dersen, sistem o thread'i uyutur ve tam 200 ms sonra uyandıracağının garantisini vermez. Sistem yoğunsa bu süre 210 ms veya 220 ms olabilir. Philo gibi milisaniyelerin hayati olduğu bir projede bu sapmalar (drift) anında ölümle sonuçlanır.

Ayrıca 200 ms boyunca thread'i tamamen uyutursan, filozof o 200 ms içinde açlıktan ölse bile, thread uyuduğu için ölüp ölmediğini kontrol edemezsin!

3. Kusursuz ft_usleep Nasıl Çalışır?
Bizim yazacağımız ft_usleep, argüman olarak doğrudan milisaniye alır. Mantığı şudur: "Hedeflenen zamana ulaşana kadar döngüde kal, ama CPU'yu %100 meşgul edip bilgisayarı dondurmamak için döngünün içinde çok çok küçük (örn: 500 mikrosaniye) standart usleep'ler at."

Klasik Yöntemin Tehlikesi (Neden Çökeriz?)
Diyelim ki Makefile'ında kural eski usül şöyle yazıyor: %.o: %.c

Kodu derledin (make). Her şey çalışıyor.

Sonra philo.h dosyasını açtın ve t_philo struct'ının içine yeni bir değişken ekledin: int is_dead;

Dosyayı kaydettin ve terminale tekrar make yazdın.

Hata Burada Başlar: make programı sadece .c dosyalarının değiştirilme saatine bakar. Sen sadece .h dosyasını değiştirdiğin için make sana şunu söyler: "make: Nothing to be done for 'all'." (Derlenecek bir şey yok).

Ornek:
# .c dosyalarını .o dosyalarına çevirirken philo.h'ı da dinliyoruz
%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@


TEST DOSYASI THREAD İÇİN: 

#include <pthread.h>
#include <stdio.h>

int counter = 0;   // paylasilan global degisken, mutex YOK
pthread_mutex_t mutex_1;

void *increment(void *arg)
{
    int i;

    i = 0;
    while (i < 100000)
    {
		pthread_mutex_lock(&mutex_1);
		counter++;   // RACE: birden fazla thread ayni anda okuyup yaziyor
		pthread_mutex_unlock(&mutex_1);
        i++;
    }
    return (NULL);
}

	

int main(void)
{
    pthread_t t1;
    pthread_t t2;
	pthread_t t3;
	
	pthread_mutex_init(&mutex_1, NULL);
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
	pthread_create(&t3, NULL, increment, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	pthread_mutex_destroy(&mutex_1);
    printf("counter: %d\n", counter);
    return (0);
}

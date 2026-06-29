***PHILO KLASORÜ İÇERİSİNDE VERİLECEK İNDİĞİNDE PHİLO KLASÖRÜ VE YANINDA READ.ME OLUCAK (KOD PROJE FİLO İÇİNDE OLACAK)

***Makefile'da PTHREAD flag ne işe yarıyor.

***Fonksiyonlar long döndürcek long long olmasına gerek yok

***Philo.h time to die, eat, sleep long olarak alınacak

***ft_atoi de - kontrol ediyor mu

***long long almak bir şeyi farkettirmez

***init.c assign fonksiyonu var if else bloğu 

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

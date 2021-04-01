
#include "network/server.h"
#include "network/client.h"
#include  "../headers/cryptosystem/signature.h"
#include <time.h>

int main()
{
#define TEST
    Transaction trans;
    trans.signature_len = 2048;
    trans.transaction_signature = "210104204132000.12410201gyhjbdrfudefhjksgwdfscwjukhhbgnj,vfdchnswbgvfbncd swdcghbngbdvnxsqdzesqdzfsdefwvdrfsxgvrdfwsgvfgtdrbfgtdxbftgdxbdfrtxgtdfrgxtdrfgxrtdgfxgxfgrfuyghigrfiukjwserdfgbvuyhjkwhgbvfhuionjfiulvgwhxsmdplkgthnyub yfjckgyn dsgvykflqk sdbjyfsekryudfgy,jnsyuredfgb syfgkiu ncfgrkjiuydb gfvwskdiuyedrngt bswki fngukwqjs fv ngwsjukyf bgbwiskudfenv wkisgfy unwkqsiuf ,ygnwkusrgy fkiuqsf bsghkuyifgviuwklsgfuwlsiofyvgbnikstehrcnfgsreduyjwnfgqkiuzsyrfn,c yqujzefngqjshefgnqu kzcyecrtgbnqsuyjkwfgliowquseygf bergiuystgfnsqiukfytg,jsgketfgvs;iekgutyfnkswg,ucvsgr gsiohefrvguhyuhigfqsdhjnbsgknjgfcrgjnk;vxgnkhjcxngj,dfwnj;tcyng; jkwxgkjvbfcgwh, nf,wvgjsk uyfdu:k;j,wcgxs hnbgfjdyuutiswgn vsr,cdsfiujlkyng,hb fdjxknfgjwhfjcvkwh,;jngvxkslgiyfvjgx,k;dsjgfnhb vhjxhgc,bn,klsvmddwfxhujkgcgbxgkn,hc, vbnx,;ck:ljv ,bn,vck l;w<x ,bnvnjcxcxnbvgc nb,x bcvn,bnxc,b v,cn xv dfhncrghbejfh,gnjhexjrsdhbfjoqrutyduikjfdcugihbnjdhdcxfngbjucrlgehbjbfdfjhyguil;g,cvbnhdjk hxfvjxdfhngfbv,x;u,jdhnbfvg hn,jvbcn,fvhjcxkdhjb nwcbv ,;xjkx ,;iugxdfhodiuflhdfghuldsfcggkhj,df,gkhjldsfg,nkdfgckng,dfcgnkdsef,jkhsqdfexwjhn fwdkfdn,jgxc,gxf sbfjgckfhnng l,kxljdfgxlnhvfjdb,;x,hbfdj;hnbvxxldjl,hnvgjks;dng vswdjk;fngcv hjk;xdf,ngvckgfxng,b nv;bf, x;cj,bgx;fdxbhj ,xnf;n :dvhwgjk;f vgwhkdw fgdjjhg,dfbnh jgdksfkk,gncfhkkjuetszrfckjrhgkdzqejhbfierlq:sckjnbhgfdikgvg;k,kredwlcngv;jk,xd gvn; gjkgjh ;frswkj;dwfgrjkfwrgjd;,fdvx;, bndwfxxdfb n,x dbfnv,; bnx,;vdfbvdxbn,xdvfbb n,vxfdbbn,vxdbdfvxbn,vdfbn, vbn,dfnb,vdfxn,bdvfxnb, dfvxbn, dfvxbn n,b dfxvnb, fdgxvrn,bfvgnb, fvdbn ,nb ,fnb ,fqshdgkjfqhgjbkqdfshgjkqdfhgjksdhjqkgfshgjbkdfghqdfsghfdqgswvhfdsghjkghdjkfwhjgfdwshjgwfdhgjwdfhjgkhjfgd,kbcxgnhkf,nvfgjhdkxtskkjx,hndgxjxcbhjfvwckjkjhn;gwvsjnkghw;dsvhjngvwdfsnkjhgdfvx d fxvbxdfv bn;,dn ,bcvf b,gn;dvxsngdfk;gsjdjfdbwvngijukswbnwdfjskbsfdjvnkbwjkmbndfsjkmgvfwjmklgndfmjkgj wxfdh,jnvcl;df,xhgvnljch x:dfbvh,lfcd:xknhf,bv xkb;gjfxh ngvc,xdfbnjkhd fgvj;xbdbfv,hj k njkfdgn,vjhf,gdxhjjkcfgxgnc,vjgkfdxgv , cndfx;,kg,vndf:w;nklhfbj nsw;bvhskrteqrlnhyjturkghuidrohgbiurdtghfbnhdrfgdf";
    trans.transaction_data.organisation_public_key = RSA_generate_key(2048, 3, NULL, NULL);
    trans.transaction_data.receiver_public_key = RSA_generate_key(2048, 3, NULL, NULL);
    trans.transaction_data.sender_public_key = RSA_generate_key(2048, 3, NULL, NULL);
    trans.transaction_data.transaction_timestamp = time(NULL);
    trans.transaction_data.amount = 1000000000000000;
    Block block;
    block.block_data.block_timestamp = time(NULL);
    block.block_data.height = 0;
    block.block_data.nb_transactions = 1;
    block.block_data.transactions = &trans;
    block.block_data.validator_public_key = RSA_generate_key(2048, 3, NULL, NULL);
    block.block_signature = "qhufncjgerdgghuin,fen,gf,nhfdhn,jfgdhn,fjgnhfgjnhkjfgcnhkjfxnhfxkjgnhkjfkxhfjcgnxkhfcgnkhjfxgnhkjxfgnhkxjfgnhkxjffffffffffffffffffffffffghnfgxnhgfxnhjxfgnhjk,xfgnh,fxgnhdfxgdnh,,nhgh,niuxui,hctgruixhgthun,gxth,nuightugihuigtv,ghuiv,huthurxuhxtgu,ngthnu,tuguiloc,iopjiocdspiowi,orei,uh,tunhitgnuicwnuierycgfirnbeiniu,frwyingdfnkjbhxlgkjfxzsdftvygbxesrdctfvygexsrdctfvygbyvsgunyrnvchrjukgshnkjcrionuhdrstcgnhftgnhgfcnh,jgfdchn,jkfgdch,njkfgdnchj,khfngdc,jhnj,kfgdchjnk,lglhkjfdxlfjhkgvlhnjdvgnlhjxlhdrfjhjlhgnxvdjrhh,lgljikfghsoigjhskoigjoikghjoisgohiogiorgfojhirgfoijhrgoijhrgoijgoijrxgoijrgoijgoirjhoigrjhoijhrgoijrdxsfgoijhgrirgjhirgijohriodxltghdiuolghiulsrghuisrghwuidrtghyruigthyrieustgyhisoreutgyhdjiughsidujfghkjhgkxjhgtiusoertyhkjiehgtreidushyriosuedhutuyirolsedhytuguiordehtgriduehtrieduohtgruieodthguieotgiuorsethiuolserislouilorsutgiulsrxotghilosdjujdrxlurthgulirdtseloiurfghuislhgtysitrghyrseoitrhtriohyruoslwxrutyhiolrsthyuugrljfdiluirdjirdujhjguirjhdfjgkhgfdkjgfhjkdfgldfglkjhlkjfxdghjxvcv,cb;nkfjlxdghjlksrethgioufdghgklfjdhgrskeljdxhgirudjtghjilrothuyilwusoretuhyilwsreuthygliusedtuhswlioerdthgysilehirulsuhirhljikswxrghriughurlxsirutghhjuidxghjkhfxnjhguhxdirghujxidhugrdiuhxdiluohgrxduirhgxdiultghyrdxuilthygxdriulthygrdxuihyrduifxhrduihytyrudxuhrtruxdruxiuirhluhriuhirhuruhixdlliusiujjugncrkhkduh,senfyguisrugnysrfgnhngrs,hnkgsjrc,hnkjnhrfh,fgrdchfrgcdhjkb,cfdghjk,jhk,bcfhj,nkbcfdghj,knfcdghjk,nfdcgjkh,nfdcgkhj,nfgcdkhj,cfkgndh,nhk,jcfdgn,hkjcfgdnhjk,cdfgnhjkcfdgnhjkcgfdnhkj,c,fgkd,knhcjfgn,khjcgfdn,hjkcfgn,hjkcgfdn,hkjcfgdn,hcfng,dn,hkcjfgdnhndnchcnrhudnuhjcdfgnhgdrcfndfcgnghjdcfgbhjdcfbghjcfdbghjdfrgbdszedsihuyeihuyjdseuhyjdseuhjyisdeuhjysdujhyidfuhjyidsxfuhjyidiujshxfsiduxhfuhijydsxfuhijydfxfhuyijeruysyubfcbgyrsecbgyhcfbyguresbuygyubgfxrbnsygueryuibgyiucyuiersuyicfnbyugersuiyfgysreuigfisuerofhdsjkbvfeiugrutccuingcyreershgnbyuuirgiughrduighdrughirfdguhirfduighdrfuighdrfuihggdrfhugigdrfuhigdgruihdrguhidrfuhidrfhuidrfguhgirfdguihdrfghuirfhdguhufgdhuifdghuifdghuifdghuidfghudguhhuidrghuidrhjudkhjdfbfdhjfdhjdfjfdhfhbfdhfdhjhjfd";
    block.signature_len = 2048;
    size_t size = 0;
    convert_blockdata_to_data(block, &size);
    return init_server();
}
package com.example.myapplication
import android.content.SharedPreferences
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.View.OnClickListener
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.databinding.adapters.ViewBindingAdapter
import com.example.myapplication.databinding.ActivityMainBinding
import okhttp3.OkHttpClient
import okhttp3.Request
import java.io.IOException


class MainActivity : AppCompatActivity() {

    private lateinit var request: Request
    private lateinit var binding: ActivityMainBinding  /*Здесь может быть что то не так ActivityMainBinding*/
    private lateinit var pref:SharedPreferences
    private val client=OkHttpClient()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding=ActivityMainBinding.inflate(layoutInflater)
      /* enableEdgeToEdge()*/
        setContentView(binding.root)
        pref=getSharedPreferences("MyPref", MODE_PRIVATE)
        OnClickSaveIp()
        getIp()
        binding.apply {
            bLed1.setOnClickListener(OnClickListener())
            bLed2.setOnClickListener(OnClickListener())
            bLed3.setOnClickListener(OnClickListener())
        }

    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.main_menu, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        /*if (item.itemId==R.id.sync)*/ post("temperature")
        return true
    }

    private fun OnClickListener():View.OnClickListener{  /*Слушатель кнопок Led и отправляет в скейтч Led1*/
        return OnClickListener(){
            when(it.id){
                R.id.bLed1->{post("led1")}
                R.id.bLed2->{post("led2")}
                R.id.bLed3->{post("led3")}
            }
        }
    }


    private fun getIp()= with(binding){ /*Для сохранения IP адреса*/
        val ip=pref.getString("ip","")
        if (ip != null){
            if(ip.isNotEmpty()) edIp.setText(ip)
        }
    }

    private fun OnClickSaveIp()= with(binding){
        bSave.setOnClickListener{
            if (edIp.text.isNotEmpty())saveIp(edIp.text.toString())
        }
    }

    private fun saveIp(ip:String){
        val editor=pref.edit()
        editor.putString("ip",ip)
        editor.apply()
    }

    private fun post(post:String){     /*Для отправления и получения данных*/
        Thread{
            request=Request.Builder().url("http://${binding.edIp.text}/$post").build()

            try{
                var response=client.newCall(request).execute()
                if(response.isSuccessful){
                   val resultText=response.body()?.string()
                    runOnUiThread {
                        val temp = resultText + "mkSv/h"
                        binding.tvTemp.text = temp
                    }
                }
            }catch (i:IOException){

            }

        }.start()
    }
}
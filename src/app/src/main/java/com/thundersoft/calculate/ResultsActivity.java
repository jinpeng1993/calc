package com.thundersoft.calculate;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.style.RelativeSizeSpan;
import android.text.style.SuperscriptSpan;
import android.util.Log;
import android.widget.LinearLayout;
import android.widget.TextView;

public class ResultsActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_results);
        LinearLayout mLinear = (LinearLayout) findViewById(R.id.linear);
        LinearLayout mLinear2 = (LinearLayout) findViewById(R.id.linear2);
        String results = getIntent().getStringExtra("results");
        Log.d("wenjie","results: "+results);
        String str = results;
//        String resulat = " ";
//        String fenmu = " ";
//        String fenzi = " ";
//        for (int i = 0; i < str.length(); i++) {
//            char  item =  str.charAt(i);
//            if (item == '/') {
//                item = ' ';
//                fenzi = resulat;
//                resulat = " ";
//            }
//            String str1 = String.valueOf(item);
//            resulat = resulat + str1;
//            fenmu = resulat;
//        }
//        String[] all=str.split("\\/");

        str = zhuanhuan(str);
        String[] all = str.split("\\/");
        String fenzi = all[0];
        String fenmu = all[1];
        int geshu;
        if (shangbiao(fenzi)) {
            geshu = mokuaishu(fenzi);
            shezhitext(geshu,fenzi,mLinear);
        }
        else if (!shangbiao(fenzi)) {
            TextView child = new TextView(this);
            child.setText(fenzi);
            mLinear.addView(child);
        }
        if (shangbiao(fenmu)) {
            geshu = mokuaishu(fenmu);
            shezhitext(geshu,fenmu,mLinear2);
        }
        else if (!shangbiao(fenmu)){
            TextView child = new TextView(this);
            child.setText(fenmu);
            mLinear2.addView(child);
        }

//        TextView textView2 = (TextView) findViewById(R.id.te2);
//        textView2.setText(fenmu);
//        TextView textView3 = (TextView) findViewById(R.id.te3);
//        int start = onWhereStart(fenzi);
//        int end = onWhereEnd(fenzi);
////        SpannableString spannableString = new SpannableString(fenzi);
//        SuperscriptSpan superscriptSpan = new SuperscriptSpan();
//
//
//
//
//        String part = str.substring(0,end);
//        String part2 = str.substring(11,fenzi.length());
//
//        Log.d("1111111111111",part2);
//
//        SpannableString part1 = new SpannableString(part);
//        part1.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
//        textView3.setText(part1);
//
//        SpannableString part3 = new SpannableString(part2);
//        start = onWhereStart(part2);
//        end = onWhereEnd(part2);
//        Log.d("start = " + start ,"end = " + end);
//        part3.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
//        textView.setText(part3);

//        part1.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
//
//        spannableString.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
//        spannableString.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_INCLUSIVE);
//        textView.setText(spannableString);

    }

    public String zhuanhuan(String string) {
        int i;
        String str1 = " ";
        String str = "";
        for (i = 0;i < string.length();i++) {
            char item = string.charAt(i);
            if (item == '^' ) {
                item = ' ';

            }
            str1 = String.valueOf(item);
            str = str+str1;
            Log.d("tag","str1====" + str1 +"str =="+str);
        }
        return str;
    }

    public int onWhereStart(String string) {
        int i;
        for (i = 0;i < string.length();i++) {
            char item = string.charAt(i);
            if (item == ' ' ) {
                return i;
            }
        }
        return i;
    }
    public int onWhereEnd(String string) {
        int i;
        for (i = 0; i < string.length(); i++) {
            char item = string.charAt(i);
            char next = string.charAt(i + 1);
            if (item == ' ' && next != '(') {
                for (int r = i; r < string.length(); r++,i++) {
                    item = string.charAt(i);
                    if (item == '+' || item == '-' || item == ')') {
                        return i;
                    }
                    else if (r== string.length()-1){
                        return i+1;
                    }
                }
            } else if (item == ' ' && next == '(') {
                for (int r = i; r < string.length(); r++, i++) {
                    item = string.charAt(i);
                    if (item == ')') {
                        return i+1;
                    }
                }
            }
        }
        return i;

    }
    public int mokuaishu(String string) {
        Log.d("tag","zzzzz");
        int i;
        int geshu = 0;
        for (i=0;i<string.length();i++) {
            char item = string.charAt(i);
            if (item == ' '){
                geshu = geshu + 1;
            }
        }
        return geshu ;
    }
    public void shezhitext(int x,String string,LinearLayout linear) {

        Log.d("tag","zoule");
        for (int i=0;i<x;i++) {
//                String str = "(123b^(3+4)+12a^3)*24c/d+34^2";
            if (i < x - 1) {
                TextView child = new TextView(this);
                int start = onWhereStart(string);
                int end = onWhereEnd(string);

                String part = string.substring(0, end);
                Log.d("tag", "strrrrr" + string);

                Log.d("tag", "str1=" + string);
//                str = str1;
                SpannableString spannableString = new SpannableString(part);
                SuperscriptSpan superscriptSpan = new SuperscriptSpan();
                spannableString.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
                spannableString.setSpan(new RelativeSizeSpan(0.7f), start, end, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                child.setText(spannableString);
                Log.d("tag", "spannableString=" + spannableString);
                linear.addView(child);
                string = string.substring(end, string.length());
            } else if (i == x - 1) {
                TextView child = new TextView(this);
                int start = onWhereStart(string);
                int end = onWhereEnd(string);
                Log.d("tag", "start =" + start + " end =" + end);
                SpannableString spannableString = new SpannableString(string);
                SuperscriptSpan superscriptSpan = new SuperscriptSpan();
                spannableString.setSpan(superscriptSpan, start, end, Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
                spannableString.setSpan(new RelativeSizeSpan(0.7f), start, end, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                child.setText(spannableString);
                linear.addView(child);
            }
        }

    }
    public boolean shangbiao(String string) {
        int r = 0;
        for (int i = 0; i < string.length(); i++) {
            char item = string.charAt(i);
            if (item == ' ') {
                r =r + 1;
            }
        }
        if (r == 0) {
            return false;
        }
        return true;
    }






    public static void actionStart(Context context, String results) {
        Intent intent = new Intent(context, ResultsActivity.class);
        intent.putExtra("results", results);
        context.startActivity(intent);

    }
}
